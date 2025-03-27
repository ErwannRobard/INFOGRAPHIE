#version 330 core

out vec4 fragColor;
uniform sampler2D colorTexture;

// TODO
void main()
{
    // vec2 texCoord = gl_FragCoord.xy / 100.0;
    // vec4 tex = texture(colorTexture, texCoord);
    // vec3 tint = vec3(0.25, 0.8, 1.0);
    // fragColor = vec4(tex.rgb * tint, tex.a);

    vec2 texCoord = gl_FragCoord.xy / 100.0;
    vec4 tex = texture(colorTexture, texCoord);
    vec3 tint = vec3(0.25, 0.8, 1.0);
    
    // Méthode alternative: addition au lieu de multiplication
    vec3 color = tex.rgb + tint;
    color = clamp(color, 0.0, 1.0);  // Limiter les valeurs entre 0 et 1
    
    fragColor = vec4(color, tex.a);
}

