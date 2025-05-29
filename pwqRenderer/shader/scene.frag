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
	//������Ч��
	bool valid;
	//�����
	float shininiess;
	//���淴��ǿ��
	float shininessStrength;
	//��͸����
	float opacity;
	//�Է���ǿ��
	float emissiveIntensity;
	//��ɫ
	vec3 clr[MAX_CLR];
	//��������
	bool hasTex[MAX_TEX];
}mat;
//����
uniform sampler2D tex[MAX_TEX];
//////////////////////////////////////////////////////////////////////////////
//���Դ
struct LightDot
{ 
	//�Ƿ�����(Ĭ��Ϊ0)
	bool enabled;
	//��Դλ��
	vec3 pos;
	//��ɫ
	vec3 clr;
	//ǿ��
	vec3 intensity;

	//��˥�� ˥��ֵF=1.0/(Kc+Kl*d+Kq*d^2)
	//������(constant)
	float Kc;
	//һ����(linear)
	float Kl;
	//������(quadratic)
	float Kq;

	//������Ӱ
	bool shadow;
	//��Ӱ��ͼ
	samplerCube texShadow;
	//Զ��ƽ�����
	float farMnear;
};
//ƽ�й�Դ
struct LightParallel
{ 
	//�Ƿ�����
	bool enabled;

	//��Դ����
	vec3 direction;
	//��ɫ
	vec3 clr;
	//ǿ��
	vec3 intensity;

	//������Ӱ
	bool shadow;
	//��Ӱ��ͼ
	sampler2D texShadow;

	//�ƹ�vp�任����
	mat4 matVP;
};
//�۹��
struct LightSpot
{
	//�Ƿ�����
	bool enabled;

	//��Դλ��
	vec3 pos;
	//��ɫ
	vec3 clr;
	//ǿ��
	vec3 intensity;

	//�۹ⷽ��
	vec3 direction;
	//�й��(��Բ׶)
	float innerCutOff;
	//�й��(��Բ׶)
	float outterCutOff;

	//��˥�� ˥��ֵF=1.0/(Kc+Kl*d+Kq*d^2)
	//������(constant)
	float Kc;
	//һ����(linear)
	float Kl;
	//������(quadratic)
	float Kq;

	//������Ӱ
	bool shadow;

	//�ƹ�vp�任����
	mat4 matVP;
};
uniform LightDot ld[MAX_LIGHT_DOT];
uniform LightParallel lp[MAX_LIGHT_PARALLEL];
uniform LightSpot ls[MAX_LIGHT_SPOT];
//////////////////////////////////////////////////////////////////////////////
struct Camera
{
	//λ��
	vec3 e;
	//�۲췽��
	vec3 g;
	//������
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
	//������ƽ��
	//if (gl_FrontFacing==false || (mat.hasTex[TEX_OPACITY]==false&&mat.opacity==0.f))
		//discard;
	if(!mat.valid)
	{
		fragOut=fsin.fragColor;
	}
	else
	{
		vec3 ret=vec3(0.f);
		//���㷨��
		vec3 normal=calcNormal();
		//������Դ
		ret+=lightDot(normal);
		//����ƽ�й�Դ
		ret+=lightParallel(normal);
		//����۹��
		ret+=lightSpot(normal);
		//�����Է���
		ret+=calcEmissive();
		//����͸����
		fragOut=calcOpacity(ret);
	}
}

//��ȡ������ɫ
vec3 getObjColor(int _texType,int _clrType)
{ 
	vec3 ret;

	if(mat.hasTex[_texType])//��ͼ
		ret=texture(tex[_texType],fsin.fragTexCoord).xyz;
	else
		ret=mat.clr[_clrType];

	return ret;
}

//����͸����
vec4 calcOpacity(vec3 _in)
{ 
	float opa;
	if(mat.hasTex[TEX_OPACITY])//͸������ͼ
		opa=texture(tex[TEX_OPACITY],fsin.fragTexCoord).x;
	else
		opa=mat.opacity;
	return vec4(_in,opa);
}

//�����Է���Ӱ�����
vec3 calcEmissive()
{
	vec3 matClr;
	matClr=getObjColor(TEX_EMISSIVE,CLR_EMISSIVE);
	return matClr*mat.emissiveIntensity;
}

//���㷨��
vec3 calcNormal()
{
	vec3 ret;
	if(mat.hasTex[TEX_NORMAL])//������ͼ
	{
		ret=normalize(texture(tex[TEX_NORMAL],fsin.fragTexCoord).rgb*2.-1.);
		ret=normalize(fsin.TBN*ret);
	}
	else
		ret=normalize(fsin.fragNormal);//��ֵ����(norm)
	return ret;
}

//����Phong��Blinn-Phong����ģ�ͼ���ƹ�
vec3 calcLight(vec3 normal,vec3 litIn,vec3 litIntensity,vec3 litClr,bool Blinn,float shadow)
{
	vec3 litOut=normalize(reflect(litIn,normal));//�������(norm)
	vec3 viewDir=normalize(-cmr.g);//�۲�����(norm)
//	vec3 viewDir=normalize(cmr.e-fsin.fragPos);//�۲�����(norm)
	vec3 halfwayDir=normalize((-litIn)+viewDir);//�������(norm)

	//���������:
	vec3 difClr;
	difClr=getObjColor(TEX_DIFFUSE,CLR_DIFFUSE);
	float diff=max(dot(-litIn,normal),0.f);//������Ӱ�쳣��
	vec3 diffuse=litIntensity.y*diff*difClr;

	//�������շ���:
	vec3 ambClr;
	ambClr=difClr;
	//ambClr=getObjColor(TEX_AMBIENT,CLR_AMBIENT);
	//if(ambClr==vec3(0.,0.,0.))ambClr=difClr;//��������ʻ���������ɫΪ��ɫ
	vec3 ambient=litIntensity.x*ambClr;

	//���淴�����:
	vec3 speClr;
	speClr=getObjColor(TEX_SPECULAR,CLR_SPECULAR);
	float shn;
	if(mat.hasTex[TEX_SHININESS])//�������ͼ
		shn=texture(tex[TEX_SHININESS],fsin.fragTexCoord).x;
	else shn=mat.shininiess;
	float spec;//���淴��Ӱ�쳣��
	if(Blinn)//ʹ��Blinn-Phong
		spec=pow(max(dot(halfwayDir,normal),0.f),shn*2);//Blinn�ﵽPhongЧ����*2
	else//ʹ��Phong
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

//���㵱ǰƬ�εĵ��Դ��Ӱֵ(1.fΪ��ȫ������Ӱ��)
float calcShadowld(int index,bool PCF)
{
	vec3 texVec=fsin.fragPos-ld[index].pos;//�ӹ�Դָ��Ƭ�ε�����
	float currDist=length(texVec);//��ǰƬ�����Դ�ľ���
	float bias=0.15;//����Ƚ�ƫ����,��������ʧ��
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
		minDist=textureCube(ld[index].texShadow,texVec).r*ld[index].farMnear;//��ֱ����,���Դ�����Ƭ�����Դ�ľ���
		shadow=currDist-bias>minDist?1.0:0.0;
	}
	return shadow;
}

//���㵱ǰƬ�ε�ƽ�й�Դ��Ӱֵ(1.fΪ��ȫ������Ӱ��)
float calcShadowlp(int index,vec3 normal,bool PCF)
{ 
	vec4 fragPosInLightSpace=lp[index].matVP*vec4(fsin.fragPos,1.f);//�ƹ��ӽ��е�Ƭ��λ��
	fragPosInLightSpace.xyz/=fragPosInLightSpace.w;//͸�ӳ����ڶ�����ɫ����ִ��
	fragPosInLightSpace=(fragPosInLightSpace+1.f)/2.f;//��-1��1ӳ�䵽0��1
	if(fragPosInLightSpace.z>1.0f)return 0.0f;
	vec2 coord=fragPosInLightSpace.xy;//��Ӱ��ͼȡ������
	float currDist=fragPosInLightSpace.z;//��Ƭ�����Դ�ľ���
	float bias=0.005;//max(0.05f*(1.0f-dot(normal,lp[index].direction)),0.005f);//����Ƚ�ƫ����,��������ʧ��

	float shadow=0.f;//��Ӱֵ,1Ϊ��Ӱ��
	float minDist=0.f;
	if(PCF)//��Ӱ�����
	{
		vec2 texelSize=1.0f/textureSize(lp[index].texShadow,0);//0����ͼ
		for(int x=-1;x<=1;x++)
			for(int y=-1;y<=1;y++)
			{
				minDist=texture(lp[index].texShadow,coord+texelSize*vec2(x,y)).r;
				shadow+=currDist-bias>minDist?1.0f:0.0f;
			}
		shadow/=9.0f;//��ƽ��ֵ
	}
	else
	{
		minDist=texture(lp[index].texShadow,coord).r;//���Դ�����Ƭ�����Դ�ľ���
		shadow=currDist-bias>minDist?1.0f:0.0f;
	}
	return shadow;
}

//���㵱ǰƬ�εľ۹����Ӱֵ
float calcShadowls(int index,bool PCF)
{	
	return 0.f;
}

//������ԴӰ�����
vec3 lightDot(vec3 normal)
{
	vec3 ret=vec3(0.,0.,0.);
	for(int i=0;i<MAX_LIGHT_DOT;i++)
	{
		if(!ld[i].enabled)continue;
		float dist=length(fsin.fragPos-ld[i].pos);//��Դ��Ƭ�ξ���
		float attenuation=1.f/(ld[i].Kc+ld[i].Kl*dist+ld[i].Kq*dist*dist);
		if(attenuation<MIN_LIGHT_VALID_ATTEN)continue;//���Ըù�Դ��Ӱ��

		vec3 litIn=normalize(fsin.fragPos-ld[i].pos);//�������(norm)
		
		float shadow=0.0f;
		if(ld[i].shadow)//������Ӱ
			shadow=calcShadowld(i,true);
		vec3 phg=calcLight(normal,litIn,ld[i].intensity,ld[i].clr,true,shadow);

		ret+=attenuation*phg;
	}
	return ret;
}

//����ƽ�й�ԴӰ�����
vec3 lightParallel(vec3 normal)
{
	vec3 ret=vec3(0.,0.,0.);
	for(int i=0;i<MAX_LIGHT_PARALLEL;i++)
	{
		if(!lp[i].enabled)continue;

		vec3 litIn=normalize(lp[i].direction);//�������(norm)
		
		float shadow=0.0f;
		if(lp[i].shadow)//������Ӱ
			shadow=calcShadowlp(i,normal,true);
		vec3 phg=calcLight(normal,litIn,lp[i].intensity,lp[i].clr,true,shadow);
		ret+=phg;
	}
	return ret;
}

//����۹��Ӱ�����
vec3 lightSpot(vec3 normal)
{
	vec3 ret=vec3(0.,0.,0.);
	for(int i=0;i<MAX_LIGHT_SPOT;i++)
	{
		if(!ls[i].enabled)continue;
		float dist=length(fsin.fragPos-ls[i].pos);//��Դ��Ƭ�ξ���
		float attenuation=1.f/(ls[i].Kc+ls[i].Kl*dist+ls[i].Kq*dist*dist);
		if(attenuation<MIN_LIGHT_VALID_ATTEN)continue;//���Ըù�Դ��Ӱ��

		vec3 litIn=normalize(fsin.fragPos-ls[i].pos);//�������(norm)
		float theta=dot(litIn,normalize(ls[i].direction));//������۹�Ʒ���н�����ֵ
		float inner=cos(ls[i].innerCutOff);
		float outter=cos(ls[i].outterCutOff);

		if(theta<outter)continue;//����Բ׶�����Ӱ��
		
		float shadow=0.0f;
		if(lp[i].shadow)
			shadow=calcShadowls(i,true);
		vec3 phg=calcLight(normal,litIn,ls[i].intensity,ls[i].clr,true,shadow);

		//�����ữ��Ե:
		float intens=1.;
		if(theta>inner)//����Բ׶��
			intens=1.;
		else//������Բ׶֮��
			intens=clamp((theta-outter)/(inner-outter),0.,1.);

		ret+=attenuation*phg*intens;
	}
	return ret;
}