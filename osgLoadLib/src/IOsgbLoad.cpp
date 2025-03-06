#include "IOsgbLoad.h"
#include "FOsgbLoad.h"

std::shared_ptr<IOsgbLoad> IOsgbLoad::MakeOSGBLoad()
{
    return std::make_shared<FOsgbLoad>();
}