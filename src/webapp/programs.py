from collections import namedtuple

from database import db_connection


ProgramVariable = namedtuple('ProgramVariable', ('name', 'type', 'struct_pack_string', 'min', 'max', 'default'))

def process_automatic(thingy):
    # Nothing to do
    pass
    return 


def process_intensity(thingy):
    table_name = thingy['program_mode']
    minimap = PROGRAMS[thingy['program_mode']]
    table_name = minimap['db_table']
    set_sql_vars = []
    for v in minimap['vars']:
        val = int(thingy['data'][v.name])
        val = val % (v.max - v.min) + v.min
        set_sql_vars.append((v.name, val))
    set_sql = ','.join(f'{k}={v}' for k,v in set_sql_vars)
    print(set_sql)
    db_connection.execute(f'''UPDATE {table_name} SET {set_sql} WHERE id=1;''')
    db_connection.commit()


def process_pulse(thingy):
    request_id = thingy['_requestId']

    print('-'*10, 'process_pulse start', request_id, '-'*10)
    print('-thingy', thingy)
    
    # Only process if this request id is greater than the last one processed
    # prev_request_id = cache.get('request_id')
    # if prev_request_id is not None and request_id < prev_request_id:
    #     print('*'*20, 'stopping early with request id / prev request id', request_id, prev_request_id, '*'*20)

    minimap = PROGRAMS[thingy['program_mode']]
    table_name = minimap['db_table']
    set_sql_vars = []
    for v in minimap['vars']:
        val = int(thingy['data'][v.name])
        val = val % (v.max - v.min) + v.min
        set_sql_vars.append((v.name, val))
    set_sql = ','.join(f'{k}={v}' for k,v in set_sql_vars)
    sql = f'''UPDATE {table_name} SET {set_sql} WHERE id=1;'''
    print('-sql', sql)
    c = db_connection.cursor()
    res = c.execute(sql)
    print('-result', res)
    res = db_connection.commit()
    print('-result', res)
    print('='*10, 'process_pulse end', request_id, '='*10)


PROGRAMS = {
    'automatic': {
        'display_name': 'Automatic',
        'process': process_automatic,
        'vars': []
    },
    'manual_intensity': {
        'display_name': 'Manual Intensity',
        'process': process_intensity,
        'db_table': 'orb_intensity',
        'vars': [
            ProgramVariable('intensity', int, 'B', 0, 255, 10),
        ]
    },
    'manual_pulse': {
        'display_name': 'Manual Pulse',
        'process': process_pulse,
        'db_table': 'orb_pulse',
        'vars': [
            ProgramVariable('low_intensity', int, 'B', 0, 255, 10),
            ProgramVariable('high_intensity', int, 'B', 0, 255, 200),
            ProgramVariable('ascending', int, 'H', 0, 0xffff, 5000),
            ProgramVariable('hold_high', int, 'H', 0, 0xffff, 200),
            ProgramVariable('descending', int, 'H', 0, 0xffff, 5000),
            ProgramVariable('hold_low', int, 'H', 0, 0xffff, 10),
        ]
    }
}
