varying lowp vec2 fragTexCoord;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform lowp float opacity;

void main(void)
{
    gl_FragColor = texture2D(texture0, fragTexCoord) *
                   texture2D(texture1, fragTexCoord) *
                   opacity;
}
