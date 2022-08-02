import sqlite3

DB_FILENAME = '/tmp/penguin.sqlite3'

db_connection = sqlite3.connect(DB_FILENAME)

def init_database(PROGRAMS, filename=DB_FILENAME):
    cursor = db_connection.cursor()
    for program_mode, program_info in PROGRAMS.items():
        print(program_info)
        if not program_info['vars']:
            print(f'Skipping {program_mode} because no vars')
            continue
        data_columns = ','.join([f'{v.name} {v.type.__name__}' for v in program_info['vars']])
        table_name = program_info['db_table']
        sql = f'''CREATE TABLE IF NOT EXISTS {table_name} (id PRIMARY KEY, {data_columns})'''        
        print(sql)
        db_connection.execute(sql)

        print('-pre fill')
        
        cursor.execute(f'''SELECT * FROM {table_name}''')
        num_rows = cursor.fetchone()
        print('num_rows - ', num_rows)
        if num_rows is not None:
            num_rows = num_rows[0]
            if num_rows:
                print(f'Table {table_name } alread created {num_rows}, skipping...')
                continue
        
        data_columns2 = ','.join([f'{v.name}={v.default}' for v in program_info['vars']])
        cols = ','.join(v.name for v in program_info['vars'])
        default_values = ','.join(str(v.default) for v in program_info['vars'])
        sql = f'''INSERT INTO {table_name} (id, {cols}) VALUES (1, {default_values})'''        
        print(sql)
        db_connection.execute(sql)
        db_connection.commit()
        print('-post fill')
        for r in db_connection.execute(f'''SELECT * FROM {table_name}'''):
            print(r)
        print('--post fill')
