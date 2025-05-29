#version 330 core
#extension GL_NV_shadow_samplers_cube:enable
//////////////////////////////////////////////////////////////////////////////
#define TEX_AMBIENT 0
#define TEX_DIFFUSE 1
#define TEX_SPECULAR 2
#define TEX_SHININESS 3
#define TEX_OPACITY 4
#define TEX_NORMAL 5
#define TEX_EMISSIVE 6
#define CLR_AMBIENT 0
#define CLR_DIFFUSE 1
#define CLR_SPECULAR 2
#define CLR_EMISSIVE 3

#define MAX_TEX		7
#define MAX_CLR		4
#define MAX_LIGHT_DOT			10
#define MAX_LIGHT_PARALLEL		10
#define MAX_LIGHT_SPOT			10

#define MIN_LIGHT_VALID_ATTEN	0.001f
//////////////////////////////////////////////////////////////////////////////
layout(std140) uniform Material
{
	//材质有效性
	bool valid;
	//反光度
	float shininiess;
	//镜面反射强度
	float shininessStrength;
	//不透明度
	float opacity;
	//自发光强度
	float emissiveIntensity;
	//颜色
	vec3 clr[MAX_CLR];
	//含有纹理
	bool hasTex[MAX_TEX];
}mat;
//纹理
uniform sampler2D tex[MAX_TEX];
//////////////////////////////////////////////////////////////////////////////
//点光源
struct LightDot
{ 
	//是否启用(默认为0)
	bool enabled;
	//光源位置
	vec3 pos;
	//颜色
	vec3 clr;
	//强度
	vec3 intensity;

	//光衰减 衰减值F=1.0/(Kc+Kl*d+Kq*d^2)
	//常数项(constant)
	float Kc;
	//一次项(linear)
	float Kl;
	//二次项(quadratic)
	float Kq;

	//启用阴影
	bool shadow;
	//阴影贴图
	samplerCube texShadow;
	//远近平面距离
	float farMnear;
};
//平行光源
struct LightParallel
{ 
	//是否启用
	bool enabled;

	//光源方向
	vec3 direction;
	//颜色
	vec3 clr;
	//强度
	vec3 intensity;

	//启用阴影
	bool shadow;
	//阴影贴图
	sampler2D texShadow;

	//灯光vp变换矩阵
	mat4 matVP;
};
//聚光灯
struct LightSpot
{
	//是否启用
	bool enabled;

	//光源位置
	vec3 pos;
	//颜色
	vec3 clr;
	//强度
	vec3 intensity;

	//聚光方向
	vec3 direction;
	//切光角(内圆锥)
	float innerCutOff;
	//切光角(外圆锥)
	float outterCutOff;

	//光衰减 衰减值F=1.0/(Kc+Kl*d+Kq*d^2)
	//常数项(constant)
	float Kc;
	//一次项(linear)
	float Kl;
	//二次项(quadratic)
	float Kq;

	//启用阴影
	bool shadow;

	//灯光vp变换矩阵
	mat4 matVP;
};
uniform LightDot ld[MAX_LIGHT_DOT];
uniform LightParallel lp[MAX_LIGHT_PARALLEL];
uniform LightSpot ls[MAX_LIGHT_SPOT];
//////////////////////////////////////////////////////////////////////////////
struct Camera
{
	//位置
	vec3 e;
	//观察方向
	vec3 g;
	//上向量
	vec3 t;
};
uniform Camera cmr;
//////////////////////////////////////////////////////////////////////////////
in VS_OUT
{
	vec3 fragPos;
	vec4 fragColor;
	vec2 fragTexCoord;
	vec3 fragNormal;
	vec3 fragTangent;
	mat3 TBN;
}fsin;
out vec4 fragOut;
//////////////////////////////////////////////////////////////////////////////
vec3 calcNormal();
vec3 calcEmissive();
vec4 calcOpacity(vec3 _in);
vec3 getObjColor(int _texType,int _clrType);
float calcShadowlp(int index,vec3 normal);
vec3 calcLight(vec3 normal,vec3 litIn,vec3 litIntensity,vec3 litClr,bool Blinn,float shadow);
vec3 lightDot(vec3 normal);
vec3 lightParallel(vec3 normal);
vec3 lightSpot(vec3 normal);

void main()
{
	//不画内平面
	//if (gl_FrontFacing==false || (mat.hasTex[TEX_OPACITY]==false&&mat.opacity==0.f))
		//discard;
	if(!mat.valid)
	{
		fragOut=fsin.fragColor;
	}
	else
	{
		vec3 ret=vec3(0.f);
		//计算法线
		vec3 normal=calcNormal();
		//计算点光源
		ret+=lightDot(normal);
		//计算平行光源
		ret+=lightParallel(normal);
		//计算聚光灯
		ret+=lightSpot(normal);
		//计算自发光
		ret+=calcEmissive();
		//计算透明度
		fragOut=calcOpacity(ret);
	}
}

//获取物体颜色
vec3 getObjColor(int _texType,int _clrType)
{ 
	vec3 ret;

	if(mat.hasTex[_texType])//贴图
		ret=texture(tex[_texType],fsin.fragTexCoord).xyz;
	else
		ret=mat.clr[_clrType];

	return ret;
}

//计算透明度
vec4 calcOpacity(vec3 _in)
{ 
	float opa;
	if(mat.hasTex[TEX_OPACITY])//透明度贴图
		opa=texture(tex[TEX_OPACITY],fsin.fragTexCoord).x;
	else
		opa=mat.opacity;
	return vec4(_in,opa);
}

//计算自发光影响分量
vec3 calcEmissive()
{
	vec3 matClr;
	matClr=getObjColor(TEX_EMISSIVE,CLR_EMISSIVE);
	return matClr*mat.emissiveIntensity;
}

//计算法线
vec3 calcNormal()
{
	vec3 ret;
	if(mat.hasTex[TEX_NORMAL])//法线贴图
	{
		ret=normalize(texture(tex[TEX_NORMAL],fsin.fragTexCoord).rgb*2.-1.);
		ret=normalize(fsin.TBN*ret);
	}
	else
		ret=normalize(fsin.fragNormal);//插值法线(norm)
	return ret;
}

//按照Phong或Blinn-Phong光照模型计算灯光
vec3 calcLight(vec3 normal,vec3 litIn,vec3 litIntensity,vec3 litClr,bool Blinn,float shadow)
{
	vec3 litOut=normalize(reflect(litIn,normal));//反射光线(norm)
	vec3 viewDir=normalize(-cmr.g);//观察向量(norm)
//	vec3 viewDir=normalize(cmr.e-fsin.fragPos);//观察向量(norm)
	vec3 halfwayDir=normalize((-litIn)+viewDir);//半程向量(norm)

	//漫反射分量:
	vec3 difClr;
	difClr=getObjColor(TEX_DIFFUSE,CLR_DIFFUSE);
	float diff=max(dot(-litIn,normal),0.f);//漫反射影响常数
	vec3 diffuse=litIntensity.y*diff*difClr;

	//环境光照分量:
	vec3 ambClr;
	ambClr=difClr;
	//ambClr=getObjColor(TEX_AMBIENT,CLR_AMBIENT);
	//if(ambClr==vec3(0.,0.,0.))ambClr=difClr;//不允许材质环境光照颜色为黑色
	vec3 ambient=litIntensity.x*ambClr;

	//镜面反射分量:
	vec3 speClr;
	speClr=getObjColor(TEX_SPECULAR,CLR_SPECULAR);
	float shn;
	if(mat.hasTex[TEX_SHININESS])//反光度贴图
		shn=texture(tex[TEX_SHININESS],fsin.fragTexCoord).x;
	else shn=mat.shininiess;
	float spec;//镜面反射影响常数
	if(Blinn)//使用Blinn-Phong
		spec=pow(max(dot(halfwayDir,normal),0.f),shn*2);//Blinn达到Phong效果须*2
	else//使用Phong
		spec=pow(max(dot(litOut,viewDir),0.f),shn);
	vec3 specular=litIntensity.z*spec*speClr*mat.shininessStrength;

	return (ambient+(diffuse+specular)*(1.0-shadow))*litClr;
}

vec3 PCFSampleDirections[20] = vec3[]
(
	vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
	vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
	vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
	vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
	vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

//计算当前片段的点光源阴影值(1.f为完全处在阴影中)
float calcShadowld(int index,bool PCF)
{
	vec3 texVec=fsin.fragPos-ld[index].pos;//从光源指向片段的向量
	float currDist=length(texVec);//当前片段与光源的距离
	float bias=0.15;//距离比较偏移量,避免纹理失真
	//if(currDist>ld[index].farMnear-0.5f)return 0.f;

	float shadow=0.f;
	float minDist=0.f;
	if(PCF)
	{
		int cntSampleDirs=20;
		float viewDist=length(cmr.e-fsin.fragPos);
		float radius=(1.0 + (viewDist / ld[index].farMnear)) / ld[index].farMnear;
		for(int i=0;i<cntSampleDirs;i++)
		{
			minDist=textureCube(ld[index].texShadow,texVec+PCFSampleDirections[i]*radius).r*ld[index].farMnear;
			if(currDist-bias>minDist)
				shadow+=1.0f;
		}
		shadow/=float(cntSampleDirs);
	}
	else
	{
		minDist=textureCube(ld[index].texShadow,texVec).r*ld[index].farMnear;//该直线上,离光源最近的片段与光源的距离
		shadow=currDist-bias>minDist?1.0:0.0;
	}
	return shadow;
}

//计算当前片段的平行光源阴影值(1.f为完全处在阴影中)
float calcShadowlp(int index,vec3 normal,bool PCF)
{ 
	vec4 fragPosInLightSpace=lp[index].matVP*vec4(fsin.fragPos,1.f);//灯光视角中的片段位置
	fragPosInLightSpace.xyz/=fragPosInLightSpace.w;//透视除法在顶点着色器后执行
	fragPosInLightSpace=(fragPosInLightSpace+1.f)/2.f;//从-1到1映射到0到1
	if(fragPosInLightSpace.z>1.0f)return 0.0f;
	vec2 coord=fragPosInLightSpace.xy;//阴影贴图取样坐标
	float currDist=fragPosInLightSpace.z;//此片段与光源的距离
	float bias=0.005;//max(0.05f*(1.0f-dot(normal,lp[index].direction)),0.005f);//距离比较偏移量,避免纹理失真

	float shadow=0.f;//阴影值,1为阴影中
	float minDist=0.f;
	if(PCF)//阴影抗锯齿
	{
		vec2 texelSize=1.0f/textureSize(lp[index].texShadow,0);//0级贴图
		for(int x=-1;x<=1;x++)
			for(int y=-1;y<=1;y++)
			{
				minDist=texture(lp[index].texShadow,coord+texelSize*vec2(x,y)).r;
				shadow+=currDist-bias>minDist?1.0f:0.0f;
			}
		shadow/=9.0f;//求平均值
	}
	else
	{
		minDist=texture(lp[index].texShadow,coord).r;//离光源最近的片段与光源的距离
		shadow=currDist-bias>minDist?1.0f:0.0f;
	}
	return shadow;
}

//计算当前片段的聚光灯阴影值
float calcShadowls(int index,bool PCF)
{	
	return 0.f;
}

//计算点光源影响分量
vec3 lightDot(vec3 normal)
{
	vec3 ret=vec3(0.,0.,0.);
	for(int i=0;i<MAX_LIGHT_DOT;i++)
	{
		if(!ld[i].enabled)continue;
		float dist=length(fsin.fragPos-ld[i].pos);//光源与片段距离
		float attenuation=1.f/(ld[i].Kc+ld[i].Kl*dist+ld[i].Kq*dist*dist);
		if(attenuation<MIN_LIGHT_VALID_ATTEN)continue;//忽略该光源的影响

		vec3 litIn=normalize(fsin.fragPos-ld[i].pos);//入射光线(norm)
		
		float shadow=0.0f;
		if(ld[i].shadow)//启用阴影
			shadow=calcShadowld(i,true);
		vec3 phg=calcLight(normal,litIn,ld[i].intensity,ld[i].clr,true,shadow);

		ret+=attenuation*phg;
	}
	return ret;
}

//计算平行光源影响分量
vec3 lightParallel(vec3 normal)
{
	vec3 ret=vec3(0.,0.,0.);
	for(int i=0;i<MAX_LIGHT_PARALLEL;i++)
	{
		if(!lp[i].enabled)continue;

		vec3 litIn=normalize(lp[i].direction);//入射光线(norm)
		
		float shadow=0.0f;
		if(lp[i].shadow)//启用阴影
			shadow=calcShadowlp(i,normal,true);
		vec3 phg=calcLight(normal,litIn,lp[i].intensity,lp[i].clr,true,shadow);
		ret+=phg;
	}
	return ret;
}

//计算聚光灯影响分量
vec3 lightSpot(vec3 normal)
{
	vec3 ret=vec3(0.,0.,0.);
	for(int i=0;i<MAX_LIGHT_SPOT;i++)
	{
		if(!ls[i].enabled)continue;
		float dist=length(fsin.fragPos-ls[i].pos);//光源与片段距离
		float attenuation=1.f/(ls[i].Kc+ls[i].Kl*dist+ls[i].Kq*dist*dist);
		if(attenuation<MIN_LIGHT_VALID_ATTEN)continue;//忽略该光源的影响

		vec3 litIn=normalize(fsin.fragPos-ls[i].pos);//入射光线(norm)
		float theta=dot(litIn,normalize(ls[i].direction));//光线与聚光灯方向夹角余弦值
		float inner=cos(ls[i].innerCutOff);
		float outter=cos(ls[i].outterCutOff);

		if(theta<outter)continue;//在外圆锥外忽略影响
		
		float shadow=0.0f;
		if(lp[i].shadow)
			shadow=calcShadowls(i,true);
		vec3 phg=calcLight(normal,litIn,ls[i].intensity,ls[i].clr,true,shadow);

		//设置柔化边缘:
		float intens=1.;
		if(theta>inner)//在内圆锥内
			intens=1.;
		else//在内外圆锥之间
			intens=clamp((theta-outter)/(inner-outter),0.,1.);

		ret+=attenuation*phg*intens;
	}
	return ret;
}