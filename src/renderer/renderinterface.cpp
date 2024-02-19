#include "renderinterface.h"

RenderFacade* RenderFacade::GetInstance()
{
    static RenderFacade instace;
    return &instace;
}

RenderFacade::RenderFacade()
{
}

RenderFacade::~RenderFacade()
{
}
