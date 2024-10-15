#pragma once

#include "duplicates.h"
#include "finder_task.h"
#include "signal.h"

class DuplicateFinder
{
public: // methods
    void findDuplicates(const FinderTask& task) {}

public: // signals
    Signal<void()> taskFinished;

private:
    FinderTask currentTask_;
    Duplicates duplicates_;
};