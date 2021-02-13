#include "scenaries.h"
#include <string>

void ScenariesCollection::PrepareScenaries(std::ifstream& scenaries_file)
{
    tasks_.clear();
    optimal_lengths_.clear();

    std::string buffer;
    std::getline(scenaries_file, buffer);

    int bucket, width, height;
    AgentTask task;
    float distance;

    while (scenaries_file >> bucket >> buffer >> width >> height >> \
        task.start_i >> task.start_j >> task.goal_i >> task.goal_j >> distance)
    {
        tasks_.push_back(task);
        optimal_lengths_.push_back(distance);
    }
}

int ScenariesCollection::GetNum() const
{
    return tasks_.size();
}

AgentTask ScenariesCollection::GetScenary(size_t scenary_index) const
{
    return tasks_.at(scenary_index);
}
