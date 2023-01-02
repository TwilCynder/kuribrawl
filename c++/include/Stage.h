#pragma once

class StageModel;

class Stage {
    const StageModel& model;

    public:
    Stage(const StageModel&);
    const StageModel& getModel() const;

};