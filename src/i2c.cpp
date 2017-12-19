#include "i2c.h"

i2c::i2c()
{
    i2c::shared_ptr<i2c_obj> init()
    {
        i2c::shared_ptr<i2c_obj> ptr(new i2c_obj);
        // Init f
        return ptr;
    }
}
