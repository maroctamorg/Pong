#include "pannel.hpp"

void Pannel::updateSize () {
    if(layout)
        layout->updateSize();
}

void Pannel::render () {
    // std::cout << "############\tCall to pannel.render()!\t############\n";
    this->UI_Element::render();
    if(layout)
        layout->render();
}