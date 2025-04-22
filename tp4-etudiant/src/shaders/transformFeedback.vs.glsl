#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 size;
layout (location = 4) in float timeToLive;

out vec3 positionMod;
out vec3 velocityMod;
out vec4 colorMod;
out vec2 sizeMod;
out float timeToLiveMod;

uniform float time;
uniform float dt;

uint seed = uint(time * 1000.0) + uint(gl_VertexID);
uint randhash( ) // entre  0 et UINT_MAX
{
    uint i=((seed++)^12345391u)*2654435769u;
    i ^= (i<<6u)^(i>>26u); i *= 2654435769u; i += (i<<5u)^(i>>12u);
    return i;
}
float random() // entre  0 et 1
{
    const float UINT_MAX = 4294967295.0;
    return float(randhash()) / UINT_MAX;
}

const float PI = 3.14159265359f;
vec3 randomInCircle(in float radius, in float height)
{
    float r = radius * sqrt(random());
    float theta = random() * 2 * PI;
    return vec3(r * cos(theta), height, r * sin(theta));
}


const float MIN_TIME_TO_LIVE = 1.7f;
const float MAX_TIME_TO_LIVE = 2.0f;
const float INITIAL_RADIUS = 0.2f;
const float INITIAL_HEIGHT = 0.0f;
const float FINAL_RADIUS = 0.5f;
const float FINAL_HEIGHT = 5.0f;

const float INITIAL_SPEED_MIN = 0.5f;
const float INITIAL_SPEED_MAX = 0.6f;

const float INITIAL_ALPHA = 0.0f;
const float ALPHA = 0.1f;
const vec3 YELLOW_COLOR = vec3(1.0f, 0.9f, 0.0f);
const vec3 ORANGE_COLOR = vec3(1.0f, 0.4f, 0.2f);
const vec3 DARK_RED_COLOR = vec3(0.1, 0.0, 0.0);

const vec3 ACCELERATION = vec3(0.0f, 0.1f, 0.0f);

void main()
{
    // TODO

    positionMod = position;
    velocityMod = velocity;
    colorMod = color;
    sizeMod = size;
    timeToLiveMod = timeToLive;

    if (dt > 0.0)
    {
        //Lorsque leur temps de vie est en dessous de 0.0, on réutilise la particule
        if (timeToLive <= 0.0)
        {
            //creation particule

            //a position initiale est aléatoire dans un cercle de rayon 0.2 à y = 0
            positionMod = randomInCircle(INITIAL_RADIUS, INITIAL_HEIGHT);

            //On trouve la direction de la vélocité avec un deuxième cercle de rayon 0.5 à y = 5.0.
            vec3 targetPos = randomInCircle(FINAL_RADIUS, FINAL_HEIGHT);
            vec3 direction = normalize(targetPos - positionMod);

            //Le module de la vélocité est dans l’intervalle [0.5, 0.6].
            float speed = mix(INITIAL_SPEED_MIN, INITIAL_SPEED_MAX, random());
            velocityMod = direction * speed;

            // Initial color: Yellow with 0 alpha
            // Reference: Assignment Part 2b ("La couleur initiale est la couleur jaune avec un alpha de 0.0.")
            colorMod = vec4(YELLOW_COLOR, INITIAL_ALPHA);

            //La taille initiale est écrasé d’un facteur 2 en x, et intact en y.
            sizeMod = vec2(0.5, 1.0);

            //Le temps de vie maximale est entre [1.7, 2.0]
            timeToLiveMod = mix(MIN_TIME_TO_LIVE, MAX_TIME_TO_LIVE, random());
        }
        else
        {
            //mise a jours particule

            //Le temps de vie est réduit de la différence de temps dt.
            timeToLiveMod = timeToLive - dt;

            //La position est modifiée avec la méthode d’Euler (position + vitesse ∗ dt)
            positionMod = position + velocity * dt;

            //update velocity => Euler integration avec acceleration constant 
            //La vitesse est aussi modifiée ... mais avec une accélération constante de 0.1 en y
            velocityMod = velocity + ACCELERATION * dt;

            //normalized lifetime ratios, approximate using the average
            //remaining life / average max life:
            float approxMaxTTL = (MIN_TIME_TO_LIVE + MAX_TIME_TO_LIVE) * 0.5;
            float lifeRatio = clamp(timeToLiveMod / approxMaxTTL, 0.0, 1.0);
            float ageRatio = 1.0 - lifeRatio;

            //update color based on ageRatio
            //Color transitions: 0.0-0.25 Yellow, 0.25-0.3 Yellow->Orange, 0.3-0.5 Orange, 0.5-1.0 Orange->Red
            vec3 updatedColor;
            if (ageRatio < 0.25) {
                updatedColor = YELLOW_COLOR;
            } else if (ageRatio < 0.3) {
                float t = smoothstep(0.25, 0.3, ageRatio); //smoothstep pour transition graduel
                updatedColor = mix(YELLOW_COLOR, ORANGE_COLOR, t);
            } else if (ageRatio < 0.5) {
                updatedColor = ORANGE_COLOR;
            } else { // ageRatio >= 0.5
                float t = smoothstep(0.5, 1.0, ageRatio);
                updatedColor = mix(ORANGE_COLOR, DARK_RED_COLOR, t);
            }

            //smoothstep fade in/out
            //Le alpha change passe de 0.0 à 0.1, puis de 0.1 à 0.0. Utiliser smoothstep ... Les intervalles sont [0.0, 0.2], puis [0.8, 1.0]
            float alphaFadeIn = smoothstep(0.0, 0.2, ageRatio);
            float alphaFadeOut = smoothstep(0.8, 1.0, ageRatio);
            //Combine fades: Start at 0, fade up to ALPHA, stay at ALPHA, fade down to 0
            float finalAlpha = mix(0.0, ALPHA, alphaFadeIn) * (1.0 - alphaFadeOut);

            colorMod = vec4(updatedColor, finalAlpha);


            //update size based on ageRatio
            //La taille original de x=0.5 et y=1.0 est multiplié d’un facteur variable de 1 à 1.5 selon temps de vie normalisé (ageRatio?)
            float sizeFactor = mix(1.0, 1.5, ageRatio);
            sizeMod = vec2(0.5, 1.0) * sizeFactor;

        }
    }
}
