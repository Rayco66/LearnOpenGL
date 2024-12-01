#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ScreenTexture;
uniform bool IfKernel;//�Ƿ�ʹ�ú�Ч��
uniform bool IfGray;//�Ƿ�ʹ�ûҶ�Ч��
uniform bool IfInverse;//�Ƿ�ʹ�÷���Ч��
uniform float Kernel[9];//�����

void main()
{    
	vec3 KernelColor  = vec3(0.0f);
	vec3 GrayColor    = vec3(0.0f);
	vec3 InverseColor = vec3(0.0f);
	vec4 ResultColor  = texture(ScreenTexture,TexCoord);


	if(IfKernel)
	{
		int index = 0;

		//vec2 TexOffset = 1.0f / textureSize(ScreenTexture,0);//�����ص㵽��Χ�˸����ص��λ��ƫ����
		vec2 TexOffset = vec2(1.0f / 300.0f);

		for(int x = -1;x <= 1;x++)
		{
			for(int y = -1;y <= 1;y++)
			{
				vec2 Offset = vec2(x,y) * TexOffset;
				KernelColor += Kernel[index] * texture(ScreenTexture,TexCoord + Offset).rgb;
				index++;
			}
		}	
		ResultColor = vec4(KernelColor,1.0f);		
	}
	if(IfGray)
	{
		GrayColor = texture(ScreenTexture,TexCoord).rgb;
		float average = 0.0f;
		if(IfKernel)
		{
			average = 0.2126 * KernelColor.r + 0.7152 * KernelColor.g + 0.0722 * KernelColor.b;			
		}
		else
		{
			average = 0.2126 * GrayColor.r + 0.7152 * GrayColor.g + 0.0722 * GrayColor.b;			
		}

		ResultColor = vec4(average,average,average,1.0f);
	}
	if(IfInverse)
	{
		if(IfKernel)
		{
			InverseColor = 1.0 - KernelColor;
		}
		else
		{
			InverseColor = 1.0 - texture(ScreenTexture,TexCoord).rgb;
		}
		ResultColor = vec4(InverseColor,1.0f);
	}

    FragColor = ResultColor;
}
