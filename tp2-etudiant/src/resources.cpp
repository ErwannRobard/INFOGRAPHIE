#include "resources.h"

#include "utils.h"

Resources::Resources()
{
    // TODO - init des shaders

    initShaderProgram(colorUniform, "shaders/colorUniform.vs.glsl", "shaders/colorUniform.fs.glsl");

    mvpLocationColorUniform = colorUniform.getUniformLoc("mvp");
    colorLocationColorUniform = colorUniform.getUniformLoc("laCouleurVertex");
}
