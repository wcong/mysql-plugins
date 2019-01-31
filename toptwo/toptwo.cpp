#include <iostream>
#include "mysql.h"
#include "mysql/udf_registration_types.h"

struct toptwo_data {
    long long first;
    long long second;
};

extern "C" bool toptwo_init(UDF_INIT *initid, UDF_ARGS *args, char *message){
    struct toptwo_data *data;

    if (args->arg_count != 1) {
        strcpy(message,
               "wrong number of arguments: toptwo() requires one arguments");
        return 1;
    }

    if (args->arg_type[0] != INT_RESULT) {
        strcpy(message,
               "wrong argument type: toptwo() requires a REAL");
        return 1;
    }

    initid->maybe_null = 0;  /* The result may be null */

    if (!(data = new (std::nothrow) toptwo_data)) {
        strcpy(message, "Couldn't allocate memory");
        return 1;
    }
    data->first = 0;
    data->second = 0;

    initid->ptr = (char *)data;

    return 0;
}

extern "C" void toptwo_deinit(UDF_INIT *initid) {
    void *void_ptr = initid->ptr;
    toptwo_data *data = static_cast<toptwo_data *>(void_ptr);
    delete data;
}

extern "C" void toptwo_clear(UDF_INIT *initid, unsigned char *,
                              unsigned char *) {
    toptwo_data *data = (struct toptwo_data *)initid->ptr;
    data->first = 0;
    data->second = 0;
}

extern "C" void toptwo_add(UDF_INIT *initid, UDF_ARGS *args, unsigned char *,
                            unsigned char *) {
    toptwo_data *data = (struct toptwo_data *)initid->ptr;
    if( args->args[0] ) {
        void *arg0 = args->args[0];
        long long value = *(static_cast<long long *>(arg0));
        if (value > data->first) {
            data->second = data->first;
            data->first = value;
        } else if (value<data->first && value > data->second) {
            data->second = value;
        }
    }
}
extern "C" long long toptwo(UDF_INIT *initid, UDF_ARGS *, unsigned char *is_null,
                         unsigned char *) {
    return ((toptwo_data *)initid->ptr)->second;
}
