#include "input_subject.hpp"
#include "input_observer.hpp"

Project::Input::InputEnum Project::Input::GetType() const {
    return this->type;
}

const glm::ivec2& Project::Input::GetPos() const {
    return this->pos;
}

Project::Input::Input(InputEnum type, const int code, const int action, const int x, const int y) {
    this->type = type;
    this->code = code;
    this->pos = {x, y};
    this->action = action;
}

Project::Input::Input(InputEnum type, const int code, const int action) {
    this->type = type;
    this->code = code;
    this->action = action;
}

void Project::InputSubject::NotifyObservers(const Input& input) {
    for (InputObserver* observer : this->observers) {
        observer->Notify(input);
    }
}

void Project::InputSubject::Subscribe(InputObserver& observer) {
    this->observers.push_back(&observer);
}

int Project::Input::GetAction() const {
    return this->action;
}

int Project::Input::GetCode() const {
    return this->code;
}