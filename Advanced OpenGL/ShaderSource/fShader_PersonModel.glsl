#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

struct Material
{
	sampler2D DiffuseColorSampler1;//漫反射贴图采样器标号(采样器：存储对应纹理单元中的纹理数据)
	sampler2D SpecularColorSampler1;//镜面反射贴图采样器标号
	float Shininess;//高光强度
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
//光照反射贴图
	/*漫反射*/
	vec3 TextureColor_Diff = texture(material.DiffuseColorSampler1,TexCoord).rgb;
	/*镜面反射*/
	vec3 TextureColor_Spec = texture(material.SpecularColorSampler1,TexCoord).rgb;


//光照衰减
	/*角度衰减*/
	vec3 LightDir = normalize(pointLight.Pos - FragPos);//光线入射方向（从图形指向光源）
	float Diff = max(dot(LightDir,normalize(Normal)),0.0f);

	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 LightDir_Reflex = reflect(-LightDir,normalize(Normal));//光线反射后的方向(从图形出发)
	float Spec = pow(max(dot(LightDir_Reflex,ViewDir),0.0f),material.Shininess);//高光强度，幂越大，高光散射越少，高光越集中

	/*距离衰减*/
	float Distance = length(pointLight.Pos - FragPos);//点光源衰减
	float Decline = 1.0 / (pointLight.constant + pointLight.linear * Distance + pointLight.quadratic * (Distance * Distance));

//光照计算
	/*环境光*/
	vec3 Ambient = pointLight.Ambient * TextureColor_Diff;
	/*漫反射*/
	vec3 Diffuse = (Diff * pointLight.Diffuse) * TextureColor_Diff;
	/*镜面反射*/
	vec3 Specular = (Spec * pointLight.Specular) * TextureColor_Spec;

	vec3 Result = (Ambient + Diffuse + Specular) * Decline;
	return Result;
}

vec3 GetLight_Direct(Material material,DirectLight directLight,vec3 Normal,vec3 FragPos,vec3 ViewPos)
{
//光照反射贴图
	/*漫反射*/
	vec3 TextureColor_Diff = texture(material.DiffuseColorSampler1, TexCoord).rgb;
	/*镜面反射*/
	vec3 TextureColor_Spec = texture(material.SpecularColorSampler1, TexCoord).rgb;

//光照衰减
	/*角度衰减*/
	vec3 LightDir = normalize(-directLight.Dir);//光线入射方向（从图形指向光源）
	float Diff = max(dot(normalize(Normal),LightDir),0.0f);//计算反射后的光线强度

	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 LightDir_Reflex = reflect(-LightDir,normalize(Normal));//光源光反射后的光线方向
	float Spec = pow(max(dot(LightDir_Reflex,ViewDir),0.0f),material.Shininess);//高光强度，幂越大，高光散射越少，高光越集中
	
//光照计算
	/*环境光*/
	vec3 Ambient =directLight.Ambient * TextureColor_Diff;//环境光RGB值
	/*漫反射*/
	vec3 Diffuse = (Diff * directLight.Diffuse) * TextureColor_Diff;//漫反射光RGB值
	/*镜面反射*/
	vec3 Specular = (Spec * directLight.Specular) * TextureColor_Spec;//镜面反射光RGB值

	vec3 Result = Ambient + Diffuse + Specular;
	return Result;	
}

vec3 GetLight_Flash(Material material,FlashLight flashLight,vec3 Normal,vec3 FragPos,vec3 ViewPos)
{
//光照反射贴图
	/*漫反射*/
	vec3 TextureColor_Diff = texture(material.DiffuseColorSampler1, TexCoord).rgb;
	/*镜面反射*/
	vec3 TextureColor_Spec = texture(material.SpecularColorSampler1, TexCoord).rgb;

//手电筒范围计算
	float theta = dot(normalize(FragPos - flashLight.Pos),normalize(flashLight.Dir));
	float epsilon = flashLight.InnerAngle - flashLight.OuterAngle;
	float intensity = clamp((theta - flashLight.OuterAngle) / epsilon,0.0f,1.0f);

//光照衰减
	/*角度衰减*/
	vec3 LightDir = normalize(flashLight.Pos - FragPos);//光线入射方向（从图形指向光源）
	float Diff = max(dot(normalize(Normal),LightDir),0.0f);//计算反射后的光线强度
	
	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 LightDir_Reflex = reflect(-LightDir,normalize(Normal));//光源光反射后的光线方向	
	float Spec = pow(max(dot(LightDir_Reflex,ViewDir),0.0f),material.Shininess);//高光强度，幂越大，高光散射越少，高光越集中
	
	/*距离衰减*/
	float Distance = length(flashLight.Pos - FragPos);//点光源衰减
	float Decline = 1.0 / (flashLight.constant + flashLight.linear * Distance + flashLight.quadratic * (Distance * Distance));

//光照计算
	/*漫反射*/
	vec3 Diffuse = (Diff * flashLight.Diffuse) * TextureColor_Diff;//漫反射光RGB值
	/*镜面反射*/
	vec3 Specular = (Spec * flashLight.Specular) * TextureColor_Spec;//镜面反射光RGB值

	vec3 Result = (Diffuse + Specular) * Decline * intensity;
	return Result;
}
