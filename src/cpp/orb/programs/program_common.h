#ifndef _PROGRAM_COMMON_H_
#define _PROGRAM_COMMON_H_


class Program {
public:
    virtual int read_data(int bytesReceived) = 0;
    virtual void update(int deltaMillis) = 0;
    virtual void render() = 0;
};


#endif // _PROGRAM_COMMON_H_
