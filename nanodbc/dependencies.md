The only dependency for nanodbc is unixodbc-dev OR iodbc-dev.

    sudo apt-get install unixodbc unixodbc-dev unixodbc-bin

You can install specific odbc drivers separatedly, like

    # install odbc drivers in ubuntu
    sudo apt-get install tdsodbc odbc-postgresql libmyodbc libsqliteodbc

ODBC drivers is manageable in DriverManager.

    # for unixodbc you can use gui
    sudo ODBCManageDataSourcesQ4
    # or console
    sudo odbcinst
    # for iodbc you can use iodbc-config
    sudo iodbc-config

This is optional to install specific DB systems to your computer

    # install mysql
    sudo apt-get install mysql-server mysql-client mysql-workbench mysql-utilities
    # install postgresql
    sudo apt-get install postgresql
    # install sqlite
    sudo apt-get install sqlite3 libsqlite3-dev libsqlite3-0