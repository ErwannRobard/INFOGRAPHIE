#include "resources.h"

#include "utils.h"

Resources::Resources()
{
    // TODO - init des shaders

    // uniform colors
    initShaderProgram(colorUniform, "shaders/colorUniform.vs.glsl", "shaders/colorUniform.fs.glsl");
    mvpLocationColorUniform = colorUniform.getUniformLoc("mat");
    colorLocationColorUniform = colorUniform.getUniformLoc("color");

    // textures
    initShaderProgram(texture, "shaders/texture.vs.glsl", "shaders/texture.fs.glsl");
    mvpLocationTexture = texture.getUniformLoc("mat");

    // tasses/assiettes
    initShaderProgram(cup, "shaders/cup.vs.glsl", "shaders/texture.fs.glsl");
    mvpLocationCup = cup.getUniformLoc("mat");
    textureIndexLocationCup = cup.getUniformLoc("textureIndex");
    isPlateLocationCup = cup.getUniformLoc("isPlate");
}
