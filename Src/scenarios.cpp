#include "scenarios.h"
#include <string>

void ScenariesCollection::PrepareScenaries(std::ifstream& scenaries_file)
{
    tasks_.clear();
    optimal_lengths_.clear();

    std::string buffer;
    std::getline(scenaries_file, buffer);

    int bucket, width, height;
    AgentTask<GridCell> task;
    float distance;

    while (scenaries_file >> bucket >> buffer >> width >> height >> \
        task.start.j >> task.start.i >> task.goal.j >> task.goal.i >> distance)
    {
        tasks_.push_back(task);
        optimal_lengths_.push_back(distance);
    }
}

size_t ScenariesCollection::GetNum() const
{
    return tasks_.size();
}

AgentTask<GridCell> ScenariesCollection::GetScenary(size_t scenary_index) const
{
    return tasks_.at(scenary_index);
}

FTYPE ScenariesCollection::GetOptimalLength(size_t scenary_index) const
{
    return optimal_lengths_.at(scenary_index);
}
