#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;                   
out vec4 FragColor;                          

struct Material
{
	sampler2D DiffuseColorSampler1;
	sampler2D SpecularColorSampler1;//�������ͼ���������
	sampler2D EmissionColorSampler1;//�������ͼ���������
	float EmissionStrength;//�����ǿ��
	float EmissionMovement;//�����ͼ���ƶ�
	float Shininess;//�߹�ǿ��
};

struct PointLight
{
	vec3 Pos;//���Դλ��

	vec3 Ambient;//���Դ�л��������
	vec3 Diffuse;//���Դ������������
	vec3 Specular;//���Դ�о��淴������

	//����˥������
	float constant;
	float linear;
	float quadratic;
};

struct DirectLight
{
	vec3 Dir;//����ⷽ��

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

struct FlashLight
{
	vec3 Pos;//�ֵ�Ͳλ��
	vec3 Dir;//�ֵ�Ͳ����

	vec3 Diffuse;
	vec3 Specular;
	
	float InnerAngle;//�ֵ����Ȧ�Ƕȣ�����ֵ��
	float OuterAngle;//�ֵ����Ȧ�Ƕȣ�����ֵ��

	//����˥������
	float constant;
	float linear;
	float quadratic;
};
uniform Material material;
uniform PointLight pointLight;
uniform DirectLight directLight;
uniform FlashLight flashLight;
uniform vec3 ViewPos;

vec3 GetLight_Point(PointLight pointLight,Material material,vec3 Normal,vec3 FragPos,vec3 ViewPos);
vec3 GetLight_Direct(DirectLight directLight,Material material,vec3 Normal,vec3 FragPos,vec3 ViewPos);
vec3 GetLight_Flash(FlashLight flashLight,Material material,vec3 Normal,vec3 FragPos,vec3 ViewPos);

vec3 GetEmission(Material material);

void main()
{
	vec3 ResultColor = vec3(0.0f);
	ResultColor += GetLight_Point(pointLight,material,Normal,FragPos,ViewPos);
	ResultColor += GetLight_Direct(directLight,material,Normal,FragPos,ViewPos);
	ResultColor += GetLight_Flash(flashLight,material,Normal,FragPos,ViewPos);
	
	FragColor = vec4(ResultColor,1.0f);
}

vec3 GetEmission(Material material)
{
	vec3 EmissionTexture = texture(material.EmissionColorSampler1,vec2(TexCoord.x,TexCoord.y + material.EmissionMovement)).rgb;
	vec3 Result = EmissionTexture * material.EmissionStrength;
	return Result;
}


vec3 GetLight_Point(PointLight pointLight,Material material,vec3 Normal,vec3 FragPos,vec3 ViewPos)
{
//���շ�����ͼ
	/*������*/
	vec3 TextureColor_Diff = texture(material.DiffuseColorSampler1,TexCoord).rgb;
	/*���淴��*/
	vec3 TextureColor_Spec = texture(material.SpecularColorSampler1,TexCoord).rgb;

//����˥��
	/*�Ƕ�˥��*/
	vec3 LightDir = normalize(pointLight.Pos - FragPos);//�������䷽�򣨴�ͼ��ָ���Դ��
	float Diff = max(dot(normalize(Normal),LightDir),0.0f);//���㷴���Ĺ���ǿ��

	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 LightDir_Reflex = reflect(-LightDir,normalize(Normal));//��Դ�ⷴ���Ĺ��߷���
	float Spec = pow(max(dot(LightDir_Reflex,ViewDir),0.0f),material.Shininess);//�߹�ǿ�ȣ���Խ�󣬸߹�ɢ��Խ�٣��߹�Խ����
	
	/*����˥��*/
	float Distance = length(pointLight.Pos - FragPos);//���Դ˥��
	float Decline = 1.0 / (pointLight.constant + pointLight.linear * Distance + pointLight.quadratic * (Distance * Distance));

//���ռ���
	/*������*/
	vec3 Ambient = TextureColor_Diff * pointLight.Ambient;//������RGBֵ
	/*������*/
	vec3 Diffuse = (Diff * TextureColor_Diff) * pointLight.Diffuse;//�������RGBֵ
	/*���淴��*/
	vec3 Specular = (Spec * TextureColor_Spec) * pointLight.Specular;//���淴���RGBֵ

	vec3 Result = (Ambient + Diffuse + Specular) * Decline;
	return Result;
}

vec3 GetLight_Direct(DirectLight directLight,Material material,vec3 Normal,vec3 FragPos,vec3 ViewPos)
{
//���շ�����ͼ
	/*������*/
	vec3 TextureColor_Diff = texture(material.DiffuseColorSampler1,TexCoord).rgb;
	/*���淴��*/
	vec3 TextureColor_Spec = texture(material.SpecularColorSampler1,TexCoord).rgb;

//����˥��
	/*�Ƕ�˥��*/
	vec3 LightDir = normalize(-directLight.Dir);//�������䷽�򣨴�ͼ��ָ���Դ��
	float Diff = max(dot(normalize(Normal),LightDir),0.0f);//���㷴���Ĺ���ǿ��

	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 LightDir_Reflex = reflect(-LightDir,normalize(Normal));//��Դ�ⷴ���Ĺ��߷���
	float Spec = pow(max(dot(LightDir_Reflex,ViewDir),0.0f),material.Shininess);//�߹�ǿ�ȣ���Խ�󣬸߹�ɢ��Խ�٣��߹�Խ����
	
//���ռ���
	/*������*/
	vec3 Ambient = TextureColor_Diff * directLight.Ambient;//������RGBֵ
	/*������*/
	vec3 Diffuse = (Diff * TextureColor_Diff) * directLight.Diffuse;//�������RGBֵ
	/*���淴��*/
	vec3 Specular = (Spec * TextureColor_Spec) * directLight.Specular;//���淴���RGBֵ

	vec3 Result = Ambient + Diffuse + Specular;
	return Result;	
}

vec3 GetLight_Flash(FlashLight flashLight,Material material,vec3 Normal,vec3 FragPos,vec3 ViewPos)
{
//���շ�����ͼ
	/*������*/
	vec3 TextureColor_Diff = texture(material.DiffuseColorSampler1,TexCoord).rgb;
	/*���淴��*/
	vec3 TextureColor_Spec = texture(material.SpecularColorSampler1,TexCoord).rgb;

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
	vec3 Diffuse = (Diff * TextureColor_Diff) * flashLight.Diffuse;//�������RGBֵ
	/*���淴��*/
	vec3 Specular = (Spec * TextureColor_Spec) * flashLight.Specular;//���淴���RGBֵ

	vec3 Result = (Diffuse + Specular) * Decline * intensity;
	return Result;
}