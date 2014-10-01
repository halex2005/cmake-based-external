#include "nanodbc.h"
#include <algorithm>
#include <cstring>
#include <iostream>

using namespace std;

void show(nanodbc::result& results);

void run_test(const nanodbc::string_type::value_type* connection_string)
{
    // Establishing connections
    nanodbc::connection connection(connection_string);
    // or connection(connection_string, timeout_seconds);
    // or connection("data source name", "username", "password"));
    // or connection("data source name", "username", "password", timeout_seconds);
    cout << "Connected with driver " << connection.driver_name().c_str() << endl;

    // Setup
    execute(connection, NANODBC_TEXT("drop table if exists public.simple_test;"));
	execute(connection, NANODBC_TEXT("create table public.simple_test (a int, b varchar(10));"));

    nanodbc::result results;

    // Direct execution
    {
        execute(connection, NANODBC_TEXT("insert into public.simple_test values (1, 'one');"));
        execute(connection, NANODBC_TEXT("insert into public.simple_test values (2, 'two');"));
        execute(connection, NANODBC_TEXT("insert into public.simple_test values (3, 'tri');"));
        execute(connection, NANODBC_TEXT("insert into public.simple_test (b) values ('z');"));
		results = execute(connection, NANODBC_TEXT("select * from public.simple_test;"));
        show(results);
    }

    // Accessing results by name, or column number
    {
		results = execute(connection, NANODBC_TEXT("select a as first, b as second from public.simple_test where a = 1;"));
        results.next();
		cout << endl << results.get<int>(NANODBC_TEXT("first")) << ", " << results.get<nanodbc::string_type>(1).c_str() << endl;
    }

    // Binding parameters
    {
        nanodbc::statement statement(connection);

        // Inserting values
		prepare(statement, NANODBC_TEXT("insert into public.simple_test (a, b) values (?, ?);"));
        const int eight_int = 8;
        statement.bind(0, &eight_int);
        const nanodbc::string_type eight_str = NANODBC_TEXT("eight");
        statement.bind(1, eight_str.c_str());
        execute(statement);

        // Inserting null values
		prepare(statement, NANODBC_TEXT("insert into public.simple_test (a, b) values (?, ?);"));
        statement.bind_null(0);
        statement.bind_null(1);
        execute(statement);

        // Inserting multiple null values
        prepare(statement, NANODBC_TEXT("insert into public.simple_test (a, b) values (?, ?);"));
        statement.bind_null(0, 2);
        statement.bind_null(1, 2);
        execute(statement, 2);

        prepare(statement, NANODBC_TEXT("select * from public.simple_test;"));
        results = execute(statement);
        show(results);
    }

    // Transactions
    {
        {
            cout << "\ndeleting all rows ... " << flush;
            nanodbc::transaction transaction(connection);
            execute(connection, NANODBC_TEXT("delete from public.simple_test;"));
            // transaction will be rolled back if we don't call transaction.commit()
        }
        results = execute(connection, NANODBC_TEXT("select count(1) from public.simple_test;"));
        results.next();
        cout << "still have " << results.get<int>(0) << " rows!" << endl;
    }

    // Batch inserting
    {
        nanodbc::statement statement(connection);
        execute(connection, NANODBC_TEXT("drop table if exists public.batch_test;"));
        execute(connection, NANODBC_TEXT("create table public.batch_test (x varchar(10), y int, z float);"));
        prepare(statement, NANODBC_TEXT("insert into public.batch_test (x, y, z) values (?, ?, ?);"));

        const std::size_t elements = 4;

		nanodbc::string_type::value_type xdata[elements][10] = {
			NANODBC_TEXT("this"),
			NANODBC_TEXT("is"),
			NANODBC_TEXT("a"),
			NANODBC_TEXT("test")
		};
        statement.bind_strings(0, xdata);

        int ydata[elements] = { 1, 2, 3, 4 };
        statement.bind(1, ydata, elements);

        float zdata[elements] = { 1.1, 2.2, 3.3, 4.4 };
        statement.bind(2, zdata, elements);

        transact(statement, elements);

        results = execute(connection, NANODBC_TEXT("select * from public.batch_test;"), 3);
        show(results);

        execute(connection, NANODBC_TEXT("drop table if exists public.batch_test;"));
    }

    // Dates and Times
    {
        execute(connection, NANODBC_TEXT("drop table if exists public.date_test;"));
        execute(connection, NANODBC_TEXT("create table public.date_test (x datetime);"));
        execute(connection, NANODBC_TEXT("insert into public.date_test values (current_timestamp);"));

        results = execute(connection, NANODBC_TEXT("select * from public.date_test;"));
        results.next();

        nanodbc::date date = results.get<nanodbc::date>(0);
        cout << endl << date.year << "-" << date.month << "-" << date.day << endl;

        results = execute(connection, NANODBC_TEXT("select * from public.date_test;"));
        show(results);

        execute(connection, NANODBC_TEXT("drop table if exists public.date_test;"));
    }

    // Inserting NULL values with a sentry
    {
        nanodbc::statement statement(connection);
        prepare(statement, NANODBC_TEXT("insert into public.simple_test (a, b) values (?, ?);"));

        const int elements = 5;
        const int a_null = 0;
        const nanodbc::string_type::value_type* b_null = NANODBC_TEXT("");
        int a_data[elements] = {0, 88, 0, 0, 0};
        nanodbc::string_type::value_type b_data[elements][10] = {
			NANODBC_TEXT(""),
			NANODBC_TEXT("non-null"),
			NANODBC_TEXT(""),
			NANODBC_TEXT(""),
			NANODBC_TEXT("")
		};

        statement.bind(0, a_data, elements, &a_null);
        statement.bind_strings(1, b_data, b_null);

        execute(statement, elements);

        nanodbc::result results = execute(connection, NANODBC_TEXT("select * from public.simple_test;"));
        show(results);
    }

    // Inserting NULL values with flags
    {
        nanodbc::statement statement(connection);
        prepare(statement, NANODBC_TEXT("insert into public.simple_test (a, b) values (?, ?);"));

        const int elements = 2;
        int a_data[elements] = {0, 42};
        nanodbc::string_type::value_type b_data[elements][10] = {
			NANODBC_TEXT(""),
			NANODBC_TEXT("every")
		};
        bool nulls[elements] = {true, false};

        statement.bind(0, a_data, elements, nulls);
        statement.bind_strings(1, b_data, nulls);

        execute(statement, elements);

        nanodbc::result results = execute(connection, NANODBC_TEXT("select * from public.simple_test;"));
        show(results);
    }

    // Cleanup
    execute(connection, NANODBC_TEXT("drop table if exists public.simple_test;"));
}

void show(nanodbc::result& results)
{
    const short columns = results.columns();
    long rows_displayed = 0;

    cout << "\nDisplaying " << results.affected_rows() << " rows "
         << "(" << results.rowset_size() << " fetched at a time):" << endl;

    // show the column names
    cout << "row\t";
    for(short i = 0; i < columns; ++i)
        cout << results.column_name(i).c_str() << "\t";
    cout << endl;

    // show the column data for each row
    while(results.next())
    {
        cout << rows_displayed++ << "\t";
        for(short col = 0; col < columns; ++col)
			cout << "(" << results.get<nanodbc::string_type>(col, NANODBC_TEXT("null")).c_str() << ")\t";
        cout << endl;
    }
}

void usage(std::ostream& out, std::string const& binary_name)
{
    out << "usage: " << binary_name << " connection_string" << std::endl;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        char* app_name = std::strrchr(argv[0], '/');
        app_name = app_name ? app_name + 1 : argv[0];
        if(0 == std::strncmp(app_name, "lt-", 3))
            app_name += 3; // remove libtool prefix
        usage(std::cerr, app_name);
        return 1;
    }

    try
    {
        run_test(nanodbc::string_type(argv[1], argv[1] + strlen(argv[1])).c_str());
    }
    catch(const exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }
}
