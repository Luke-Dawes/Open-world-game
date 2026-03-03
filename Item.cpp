#include "Item.h"

Item::Item(const std::string& nam, std::shared_ptr<Shader> shade, glm::vec3 pos, int val)
	: name(nam), shader(shade), position(pos), value(value) 

{}

void Item::draw(const glm::mat4& view, const glm::mat4& projection) {

    glm::mat4 model = glm::mat4(1.f);

    model = glm::translate(model, position);
    //model = glm::scale(model, glm::vec3(1.f)); ///swapping translate and scale breaks it (maybe it scales the translation?)

    shader->use();
    shader->setMat4("model", model); 
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    mesh->draw();

}