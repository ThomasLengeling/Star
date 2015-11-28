#version 150

uniform mat4	ciModelViewProjection;

uniform vec3 eyePos;
uniform vec3 pos;
uniform float radius;

in vec4		    ciPosition;
in vec4	    	ciColor;
in vec2		    ciTexCoord0;

out vec4 vColor;
out vec2 vTexCoord0;
out vec4 vVertex;
out vec3 vNormal;
out vec3 vEyeDir;

void main()
{
	vVertex			= vec4( ciPosition );
	vNormal			= gl_Normal;//normalize( vec3( mMatrix * vec4( gl_Normal, 0.0 ) ) );
	vColor			= gl_Color;

	vVertex.xyz		*= radius;
	vEyeDir			= normalize( eyePos - vVertex.xyz );

	gl_Position		= mvpMatrix * ciPosition;
	gl_TexCoord[0]	= gl_MultiTexCoord0;
}
