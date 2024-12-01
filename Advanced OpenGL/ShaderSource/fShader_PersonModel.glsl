#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

struct Material
{
	sampler2D DiffuseColorSampler1;//��������ͼ���������(���������洢��Ӧ����Ԫ�е���������)
	sampler2D SpecularColorSampler1;//���淴����ͼ���������
	float Shininess;//�߹�ǿ��
};

struct PointLight
{
	vec3 Pos;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirectLight
{
	vec3 Dir;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;	
};

struct FlashLight
{
	vec3 Pos;
	vec3 Dir;

	vec3 Diffuse;
	vec3 Specular;

	float constant;
	float linear;
	float quadratic;

	float InnerAngle;
	float OuterAngle;
};

uniform Material material;
uniform PointLight pointLight;
uniform DirectLight directLight;
uniform FlashLight flashLight;
uniform vec3 ViewPos;

void GetLight_Point();
void GetLight_Direct();
void GetLight_Flash();

vec3 GetLight_Point(Material material,PointLight pointLight,vec3 Normal,vec3 FragPos,vec3 ViewPos);
vec3 GetLight_Direct(Material material,DirectLight directLight,vec3 Normal,vec3 FragPos,vec3 ViewPos);
vec3 GetLight_Flash(Material material,FlashLight flashLight,vec3 Normal,vec3 FragPos,vec3 ViewPos);


void main()
{    
	vec3 ResultColor = vec3(0.0f);
	ResultColor += GetLight_Point(material,pointLight,Normal,FragPos,ViewPos);
	ResultColor += GetLight_Direct(material,directLight,Normal,FragPos,ViewPos);
	ResultColor += GetLight_Flash(material,flashLight,Normal,FragPos,ViewPos);

    FragColor = vec4(ResultColor,1.0f);
}



vec3 GetLight_Point(Material material,PointLight pointLight,vec3 Normal,vec3 FragPos,vec3 ViewPos)
{
//���շ�����ͼ
	/*������*/
	vec3 TextureColor_Diff = texture(material.DiffuseColorSampler1,TexCoord).rgb;
	/*���淴��*/
	vec3 TextureColor_Spec = texture(material.SpecularColorSampler1,TexCoord).rgb;


//����˥��
	/*�Ƕ�˥��*/
	vec3 LightDir = normalize(pointLight.Pos - FragPos);//�������䷽�򣨴�ͼ��ָ���Դ��
	float Diff = max(dot(LightDir,normalize(Normal)),0.0f);

	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 LightDir_Reflex = reflect(-LightDir,normalize(Normal));//���߷����ķ���(��ͼ�γ���)
	float Spec = pow(max(dot(LightDir_Reflex,ViewDir),0.0f),material.Shininess);//�߹�ǿ�ȣ���Խ�󣬸߹�ɢ��Խ�٣��߹�Խ����

	/*����˥��*/
	float Distance = length(pointLight.Pos - FragPos);//���Դ˥��
	float Decline = 1.0 / (pointLight.constant + pointLight.linear * Distance + pointLight.quadratic * (Distance * Distance));

//���ռ���
	/*������*/
	vec3 Ambient = pointLight.Ambient * TextureColor_Diff;
	/*������*/
	vec3 Diffuse = (Diff * pointLight.Diffuse) * TextureColor_Diff;
	/*���淴��*/
	vec3 Specular = (Spec * pointLight.Specular) * TextureColor_Spec;

	vec3 Result = (Ambient + Diffuse + Specular) * Decline;
	return Result;
}

vec3 GetLight_Direct(Material material,DirectLight directLight,vec3 Normal,vec3 FragPos,vec3 ViewPos)
{
//���շ�����ͼ
	/*������*/
	vec3 TextureColor_Diff = texture(material.DiffuseColorSampler1, TexCoord).rgb;
	/*���淴��*/
	vec3 TextureColor_Spec = texture(material.SpecularColorSampler1, TexCoord).rgb;

//����˥��
	/*�Ƕ�˥��*/
	vec3 LightDir = normalize(-directLight.Dir);//�������䷽�򣨴�ͼ��ָ���Դ��
	float Diff = max(dot(normalize(Normal),LightDir),0.0f);//���㷴���Ĺ���ǿ��

	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 LightDir_Reflex = reflect(-LightDir,normalize(Normal));//��Դ�ⷴ���Ĺ��߷���
	float Spec = pow(max(dot(LightDir_Reflex,ViewDir),0.0f),material.Shininess);//�߹�ǿ�ȣ���Խ�󣬸߹�ɢ��Խ�٣��߹�Խ����
	
//���ռ���
	/*������*/
	vec3 Ambient =directLight.Ambient * TextureColor_Diff;//������RGBֵ
	/*������*/
	vec3 Diffuse = (Diff * directLight.Diffuse) * TextureColor_Diff;//�������RGBֵ
	/*���淴��*/
	vec3 Specular = (Spec * directLight.Specular) * TextureColor_Spec;//���淴���RGBֵ

	vec3 Result = Ambient + Diffuse + Specular;
	return Result;	
}

vec3 GetLight_Flash(Material material,FlashLight flashLight,vec3 Normal,vec3 FragPos,vec3 ViewPos)
{
//���շ�����ͼ
	/*������*/
	vec3 TextureColor_Diff = texture(material.DiffuseColorSampler1, TexCoord).rgb;
	/*���淴��*/
	vec3 TextureColor_Spec = texture(material.SpecularColorSampler1, TexCoord).rgb;

//�ֵ�Ͳ��Χ����
	float theta = dot(normalize(FragPos - flashLight.Pos),normalize(flashLight.Dir));
	float epsilon = flashLight.InnerAngle - flashLight.OuterAngle;
	float intensity = clamp((theta - flashLight.OuterAngle) / epsilon,0.0f,1.0f);

//����˥��
	/*�Ƕ�˥��*/
	vec3 LightDir = normalize(flashLight.Pos - FragPos);//�������䷽�򣨴�ͼ��ָ���Դ��
	float Diff = max(dot(normalize(Normal),LightDir),0.0f);//���㷴���Ĺ���ǿ��
	
	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 LightDir_Reflex = reflect(-LightDir,normalize(Normal));//��Դ�ⷴ���Ĺ��߷���	
	float Spec = pow(max(dot(LightDir_Reflex,ViewDir),0.0f),material.Shininess);//�߹�ǿ�ȣ���Խ�󣬸߹�ɢ��Խ�٣��߹�Խ����
	
	/*����˥��*/
	float Distance = length(flashLight.Pos - FragPos);//���Դ˥��
	float Decline = 1.0 / (flashLight.constant + flashLight.linear * Distance + flashLight.quadratic * (Distance * Distance));

//���ռ���
	/*������*/
	vec3 Diffuse = (Diff * flashLight.Diffuse) * TextureColor_Diff;//�������RGBֵ
	/*���淴��*/
	vec3 Specular = (Spec * flashLight.Specular) * TextureColor_Spec;//���淴���RGBֵ

	vec3 Result = (Diffuse + Specular) * Decline * intensity;
	return Result;
}
