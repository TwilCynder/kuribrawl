#include "Display/DynamicPlaneDrawable.h"

DynamicPlaneDrawable::DynamicPlaneDrawable(): 
    DynamicPlaneDrawable(DynamicPlaneDrawable::Layer::MIDDLE, 0)
{}

DynamicPlaneDrawable::DynamicPlaneDrawable(const DynamicPlaneDrawable::Layer layer, const int level): 
    layer(layer),
    level(level)
{}

void DynamicPlaneDrawable::resetDepth(){
    layer = Layer::MIDDLE;
}

void DynamicPlaneDrawable::setLayer(Layer layer_){
    layer = layer_;
}

void DynamicPlaneDrawable::setLevel(const int level_){
    level = level_;
}

void DynamicPlaneDrawable::incrLevel(){
    level++;
}