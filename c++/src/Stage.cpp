#include "Stage.h"
#include "StageModel.h"

Stage::Stage(const StageModel& m) :
    model(m)
{}

const StageModel& Stage::getModel() const {
    return model;
}