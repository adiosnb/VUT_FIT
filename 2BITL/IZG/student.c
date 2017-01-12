/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG
 * spanel@fit.vutbr.cz
 *
 * $Id:$
 */

#include "student.h"
#include "transform.h"
#include "fragment.h"

#include <memory.h>
#include <math.h>

/***************************************************************\
 * FPS counter
 */

#define FRAME_VALUES 10
Uint32 frametimes[FRAME_VALUES];
Uint32 frametimelast;
Uint32 framecount;
float framespersecond;

void fpsinit() {

	memset(frametimes, 0, sizeof(frametimes));
	framecount = 0;
	framespersecond = 0;
	frametimelast = SDL_GetTicks();

}

void fps() {

	Uint32 frametimesindex;
	Uint32 getticks;
	Uint32 count;
	Uint32 i;
	frametimesindex = framecount % FRAME_VALUES;
	getticks = SDL_GetTicks();
	frametimes[frametimesindex] = getticks - frametimelast;
	frametimelast = getticks;
	framecount++;
	if (framecount < FRAME_VALUES) {
		count = framecount;
	}
	else {
		count = FRAME_VALUES;
	}

	framespersecond = 0;
	for (i = 0; i < count; i++) {
		framespersecond += frametimes[i];
	}

	framespersecond /= count;
	framespersecond = 1000.f / framespersecond;

}



/*****************************************************************************
 * Globalni promenne a konstanty
 */

/* Typ/ID rendereru (nemenit) */
const int STUDENT_RENDERER = 1;

S_Material      MAT_WHITE_AMBIENT = {1.0,1.0,1.0,1};
S_Material      MAT_WHITE_DIFFUSE  ={1.0,1.0,1.0,1};
S_Material      MAT_WHITE_SPECULAR = {1.0,1.0,1.0,1};

float snimok = 0;


/*****************************************************************************
 * Funkce vytvori vas renderer a nainicializuje jej
 */

S_Renderer *studrenCreate() {
	S_StudentRenderer *renderer = (S_StudentRenderer *) malloc(sizeof(S_StudentRenderer));
	IZG_CHECK(renderer, "Cannot allocate enough memory");

	/* inicializace default rendereru */
	renderer->base.type = STUDENT_RENDERER;
	renInit(&renderer->base);

	/* nastaveni ukazatelu na upravene funkce */
	/* napr. renderer->base.releaseFunc = studrenRelease; */
	renderer->base.releaseFunc = studrenRelease;
	renderer->base.projectTriangleFunc = studrenProjectTriangle;

	/* inicializace nove pridanych casti */
	renderer->bit_map.data = loadBitmap(TEXTURE_FILENAME,&renderer->bit_map.y,&renderer->bit_map.x);

	fpsinit();

	return (S_Renderer *) renderer;
}

/*****************************************************************************
 * Funkce korektne zrusi renderer a uvolni pamet
 */

void studrenRelease(S_Renderer **ppRenderer) {
	S_StudentRenderer *renderer;


	if (ppRenderer && *ppRenderer) {
		/* ukazatel na studentsky renderer */
		renderer = (S_StudentRenderer *) (*ppRenderer);

		/* pripadne uvolneni pameti */
		free(renderer->bit_map.data);

		/* fce default rendereru */
		renRelease(ppRenderer);
	}
}

/******************************************************************************
 * Nova fce pro rasterizaci trojuhelniku s podporou texturovani
 * Upravte tak, aby se trojuhelnik kreslil s texturami
 * (doplnte i potrebne parametry funkce - texturovaci souradnice, ...)
 * v1, v2, v3 - ukazatele na vrcholy trojuhelniku ve 3D pred projekci
 * n1, n2, n3 - ukazatele na normaly ve vrcholech ve 3D pred projekci
 * x1, y1, ... - vrcholy trojuhelniku po projekci do roviny obrazovky
 */

void studrenDrawTriangle(S_Renderer *pRenderer,
						 S_Coords *v1, S_Coords *v2, S_Coords *v3,
						 S_Coords *n1, S_Coords *n2, S_Coords *n3,
						 int x1, int y1,
						 int x2, int y2,
						 int x3, int y3,
						 S_Triangle * triangle
) {
	int minx, miny, maxx, maxy;
	int a1, a2, a3, b1, b2, b3, c1, c2, c3;
	int s1, s2, s3;
	int x, y, e1, e2, e3;
	double alpha, beta, gamma, w1, w2, w3, z, u,v;
	S_RGBA col1, col2, col3, color, get_color;

	IZG_ASSERT(pRenderer && v1 && v2 && v3 && n1 && n2 && n3);

	/* vypocet barev ve vrcholech */

	col1 = pRenderer->calcReflectanceFunc(pRenderer, v1, n1);
	col2 = pRenderer->calcReflectanceFunc(pRenderer, v2, n2);
	col3 = pRenderer->calcReflectanceFunc(pRenderer, v3, n3);


	/* obalka trojuhleniku */
	minx = MIN(x1, MIN(x2, x3));
	maxx = MAX(x1, MAX(x2, x3));
	miny = MIN(y1, MIN(y2, y3));
	maxy = MAX(y1, MAX(y2, y3));

	/* oriznuti podle rozmeru okna */
	miny = MAX(miny, 0);
	maxy = MIN(maxy, pRenderer->frame_h - 1);
	minx = MAX(minx, 0);
	maxx = MIN(maxx, pRenderer->frame_w - 1);

	/* Pineduv alg. rasterizace troj.
	   hranova fce je obecna rovnice primky Ax + By + C = 0
	   primku prochazejici body (x1, y1) a (x2, y2) urcime jako
	   (y1 - y2)x + (x2 - x1)y + x1y2 - x2y1 = 0 */

	/* normala primek - vektor kolmy k vektoru mezi dvema vrcholy, tedy (-dy, dx) */
	a1 = y1 - y2;
	a2 = y2 - y3;
	a3 = y3 - y1;
	b1 = x2 - x1;
	b2 = x3 - x2;
	b3 = x1 - x3;

	/* koeficient C */
	c1 = x1 * y2 - x2 * y1;
	c2 = x2 * y3 - x3 * y2;
	c3 = x3 * y1 - x1 * y3;

	/* vypocet hranove fce (vzdalenost od primky) pro protejsi body */
	s1 = a1 * x3 + b1 * y3 + c1;
	s2 = a2 * x1 + b2 * y1 + c2;
	s3 = a3 * x2 + b3 * y2 + c3;

	if (!s1 || !s2 || !s3) {
		return;
	}

	/* normalizace, aby vzdalenost od primky byla kladna uvnitr trojuhelniku */
	if (s1 < 0) {
		a1 *= -1;
		b1 *= -1;
		c1 *= -1;
	}
	if (s2 < 0) {
		a2 *= -1;
		b2 *= -1;
		c2 *= -1;
	}
	if (s3 < 0) {
		a3 *= -1;
		b3 *= -1;
		c3 *= -1;
	}

	/* koeficienty pro barycentricke souradnice */
	alpha = 1.0 / ABS(s2);
	beta = 1.0 / ABS(s3);
	gamma = 1.0 / ABS(s1);

	/* vyplnovani... */
	for (y = miny; y <= maxy; ++y) {
		/* inicilizace hranove fce v bode (minx, y) */
		e1 = a1 * minx + b1 * y + c1;
		e2 = a2 * minx + b2 * y + c2;
		e3 = a3 * minx + b3 * y + c3;

		for (x = minx; x <= maxx; ++x) {
			if (e1 >= 0 && e2 >= 0 && e3 >= 0) {
				/* interpolace pomoci barycentrickych souradnic
				   e1, e2, e3 je aktualni vzdalenost bodu (x, y) od primek */
				w1 = alpha * e2;
				w2 = beta * e3;
				w3 = gamma * e1;


				/* interpolace z-souradnice */
				z = w1 * v1->z + w2 * v2->z + w3 * v3->z ;

				double d1 = w1/h[0];
				double d2 = w2/h[1];
				double d3 = w3/h[2];
				double spodok = (d1 + d2 + d3);
				u = (d1 * triangle->t[0].x + d2 * triangle->t[1].x + d3 * triangle->t[2].x)/spodok;
				v = (d1 * triangle->t[0].y + d2 * triangle->t[1].y + d3 * triangle->t[2].y)/spodok;


				get_color = studrenTextureValue((S_StudentRenderer*)pRenderer,u,v);

				/* interpolace barvy */
				color.red = ROUND2BYTE(w1 * col1.red + w2 * col2.red + w3 * col3.red);
				color.green = ROUND2BYTE(w1 * col1.green + w2 * col2.green + w3 * col3.green);
				color.blue = ROUND2BYTE(w1 * col1.blue + w2 * col2.blue + w3 * col3.blue);
				color.alpha = 255;

				color.red = ROUND2BYTE((color.red*get_color.red)>>8);
				color.green = ROUND2BYTE((color.green*get_color.green)>>8);
				color.blue = ROUND2BYTE((color.blue*get_color.blue)>>8);


				/* vykresleni bodu */
				if (z < DEPTH(pRenderer, x, y)) {
					PIXEL(pRenderer, x, y) = color;
					DEPTH(pRenderer, x, y) = z;
				}
			}

			/* hranova fce o pixel vedle */
			e1 += a1;
			e2 += a2;
			e3 += a3;
		}
	}

}

/******************************************************************************
 * Vykresli i-ty trojuhelnik n-teho klicoveho snimku modelu
 * pomoci nove fce studrenDrawTriangle()
 * Pred vykreslenim aplikuje na vrcholy a normaly trojuhelniku
 * aktualne nastavene transformacni matice!
 * Upravte tak, aby se model vykreslil interpolovane dle parametru n
 * (cela cast n udava klicovy snimek, desetinna cast n parametr interpolace
 * mezi snimkem n a n + 1)
 * i - index trojuhelniku
 * n - index klicoveho snimku (float pro pozdejsi interpolaci mezi snimky)
 */

void studrenProjectTriangle(S_Renderer *pRenderer, S_Model *pModel, int i, float n) {
	S_Coords    aa, bb, cc;             /* souradnice vrcholu po transformaci */
	S_Coords    aa_n1, bb_n1, cc_n1;    /* souradnice vrcholu po transformaci pre n + 1*/
	S_Coords    naa, nbb, ncc;          /* normaly ve vrcholech po transformaci */
	S_Coords    naa_n1, nbb_n1, ncc_n1; /* normaly ve vrcholech po transformaci */
	S_Coords    nn;                     /* normala trojuhelniku po transformaci */
	S_Coords    nn_n1;                     /* normala trojuhelniku po transformaci */
	int         u1, v1, u2, v2, u3, v3; /* souradnice vrcholu po projekci do roviny obrazovky */
	S_Triangle  * triangle;
	int         vertexOffset, normalOffset; /* offset pro vrcholy a normalove vektory trojuhelniku */
	int 		vertex_offset_n_1,normal_offset_n_1;
	int         i0, i1, i2, in;             /* indexy vrcholu a normaly pro i-ty trojuhelnik n-teho snimku */
	int 		i0_n1,i1_n1,i2_n1,in_n1;

	double koeficient = n - ((int)n);

	IZG_ASSERT(pRenderer && pModel && i >= 0 && i < trivecSize(pModel->triangles) && n >= 0 );

	/* z modelu si vytahneme i-ty trojuhelnik */
	triangle = trivecGetPtr(pModel->triangles, i);

	/* ziskame offset pro vrcholy n-teho snimku */
	vertexOffset = (((int) n) % pModel->frames) * pModel->verticesPerFrame;
	vertex_offset_n_1 = (((int) (n + 1)) % pModel->frames) * pModel->verticesPerFrame;


	/* ziskame offset pro normaly trojuhelniku n-teho snimku */
	normalOffset = (((int) n) % pModel->frames) * pModel->triangles->size;
	normal_offset_n_1= (((int) (n+1)) % pModel->frames) * pModel->triangles->size;

	/* indexy vrcholu pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
	i0 = triangle->v[ 0 ] + vertexOffset;
	i1 = triangle->v[ 1 ] + vertexOffset;
	i2 = triangle->v[ 2 ] + vertexOffset;

	i0_n1 = triangle->v[ 0 ] + vertex_offset_n_1;
	i1_n1 = triangle->v[ 1 ] + vertex_offset_n_1;
	i2_n1 = triangle->v[ 2 ] + vertex_offset_n_1;



	/* index normaloveho vektoru pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
	in = triangle->n + normalOffset;

	in_n1 = triangle->n + normal_offset_n_1;

	/* transformace vrcholu matici model */
	trTransformVertex(&aa, cvecGetPtr(pModel->vertices, i0));
	trTransformVertex(&bb, cvecGetPtr(pModel->vertices, i1));
	trTransformVertex(&cc, cvecGetPtr(pModel->vertices, i2));

	trTransformVertex(&aa_n1, cvecGetPtr(pModel->vertices, i0_n1));
	trTransformVertex(&bb_n1, cvecGetPtr(pModel->vertices, i1_n1));
	trTransformVertex(&cc_n1, cvecGetPtr(pModel->vertices, i2_n1));


	//TODO:optimalizovat ak sa da
	aa.x += (aa_n1.x-aa.x)*koeficient;
	aa.y += (aa_n1.y-aa.y)*koeficient;
	aa.z += (aa_n1.z-aa.z)*koeficient;

	bb.x += (bb_n1.x - bb.x)*koeficient;
	bb.y += (bb_n1.y - bb.y)*koeficient;
	bb.z += (bb_n1.z - bb.z)*koeficient;

	cc.x += (cc_n1.x - cc.x)*koeficient;
	cc.y += (cc_n1.y - cc.y)*koeficient;
	cc.z += (cc_n1.z - cc.z)*koeficient;


	/* promitneme vrcholy trojuhelniku na obrazovku */
	h[0] = trProjectVertex(&u1, &v1, &aa);
	h[1] = trProjectVertex(&u2, &v2, &bb);
	h[2] = trProjectVertex(&u3, &v3, &cc);

	/* pro osvetlovaci model transformujeme take normaly ve vrcholech */
	trTransformVector(&naa, cvecGetPtr(pModel->normals, i0));
	trTransformVector(&nbb, cvecGetPtr(pModel->normals, i1));
	trTransformVector(&ncc, cvecGetPtr(pModel->normals, i2));

	trTransformVector(&naa_n1, cvecGetPtr(pModel->normals, i0_n1));
	trTransformVector(&nbb_n1, cvecGetPtr(pModel->normals, i1_n1));
	trTransformVector(&ncc_n1, cvecGetPtr(pModel->normals, i2_n1));

	naa.x += (naa_n1.x - naa.x)*koeficient;
	naa.y += (naa_n1.y - naa.y)*koeficient;
	naa.z += (naa_n1.z - naa.z)*koeficient;

	nbb.x += (nbb_n1.x - nbb.x)*koeficient;
	nbb.y += (nbb_n1.y - nbb.y)*koeficient;
	nbb.z += (nbb_n1.z - nbb.z)*koeficient;

	ncc.x += (ncc_n1.x - ncc.x)*koeficient;
	ncc.y += (ncc_n1.y - ncc.y)*koeficient;
	ncc.z += (ncc_n1.z - ncc.z)*koeficient;

	/* normalizace normal */
	coordsNormalize(&naa);
	coordsNormalize(&nbb);
	coordsNormalize(&ncc);

	/* transformace normaly trojuhelniku matici model */
	trTransformVector(&nn, cvecGetPtr(pModel->trinormals, in));
	trTransformVector(&nn_n1, cvecGetPtr(pModel->trinormals, in_n1));

	nn.x += (nn_n1.x - nn.x)*koeficient;
	nn.y += (nn_n1.y - nn.y)*koeficient;
	nn.z += (nn_n1.z - nn.z)*koeficient;

	/* normalizace normaly */
	coordsNormalize(&nn);

	/* je troj. privraceny ke kamere, tudiz viditelny? */
	if( !renCalcVisibility(pRenderer, &aa, &nn) )
	{
		// odvracene troj. vubec nekreslime
		return;
	}

	/* rasterizace trojuhelniku */
	studrenDrawTriangle(pRenderer,
					&aa, &bb, &cc,
					&naa, &nbb, &ncc,
					u1, v1, u2, v2, u3, v3,
					triangle
	);

}

/******************************************************************************
* Vraci hodnotu v aktualne nastavene texture na zadanych
* texturovacich souradnicich u, v
* Pro urceni hodnoty pouziva bilinearni interpolaci
* Pro otestovani vraci ve vychozim stavu barevnou sachovnici dle uv souradnic
* u, v - texturovaci souradnice v intervalu 0..1, ktery odpovida sirce/vysce textury
*/

S_RGBA studrenTextureValue(S_StudentRenderer *pRenderer, double u, double v) {
/*
	unsigned char c = ROUND2BYTE(((fmod(u * 10.0, 1.0) > 0.5) ^ (fmod(v * 10.0, 1.0) < 0.5)) * 255);
	return makeColor(c, 255 - c, 0);
*/
	S_RGBA col1,col2,col3,col4,col_ret;

	if (isnan(u) || isnan(v)){
		return makeBlackColor();
	}

	int x = (int)(v*pRenderer->bit_map.y);
	int y = (int)(u*pRenderer->bit_map.x);

	col1 = pRenderer->bit_map.data[(y*pRenderer->bit_map.x)+x];
	col2 = pRenderer->bit_map.data[(y*pRenderer->bit_map.x)+x+1];
	col3 = pRenderer->bit_map.data[((y+1)*pRenderer->bit_map.x)+x];
	col4 = pRenderer->bit_map.data[((y+1)*pRenderer->bit_map.x)+x+1];

	col_ret.red = ROUND2BYTE(((int)(col1.red + col2.red + col3.red + col4.red))>>2);
	col_ret.green = ROUND2BYTE((int)(col1.green + col2.green + col3.green + col4.green)>>2);
	col_ret.blue = ROUND2BYTE((int)(col1.blue + col2.blue + col3.blue + col4.blue)>>2);

	return col_ret;

}

/******************************************************************************
 ******************************************************************************
 * Funkce pro vyrenderovani sceny, tj. vykresleni modelu
 * Upravte tak, aby se model vykreslil animovane
 * (volani renderModel s aktualizovanym parametrem n)
 */

void renderStudentScene(S_Renderer *pRenderer, S_Model *pModel) {
	/* test existence frame bufferu a modelu */
	IZG_ASSERT(pModel && pRenderer);

	/* nastavit projekcni matici */
	trProjectionPerspective(pRenderer->camera_dist, pRenderer->frame_w, pRenderer->frame_h);

	/* vycistit model matici */
	trLoadIdentity();

	/* nejprve nastavime posuv cele sceny od/ke kamere */
	trTranslate(0.0, 0.0, pRenderer->scene_move_z);

	/* nejprve nastavime posuv cele sceny v rovine XY */
	trTranslate(pRenderer->scene_move_x, pRenderer->scene_move_y, 0.0);

	/* natoceni cele sceny - jen ve dvou smerech - mys je jen 2D... :( */
	trRotateX(pRenderer->scene_rot_x);
	trRotateY(pRenderer->scene_rot_y);

	/* nastavime material */
	renMatAmbient(pRenderer, &MAT_WHITE_AMBIENT);
	renMatDiffuse(pRenderer, &MAT_WHITE_DIFFUSE);
	renMatSpecular(pRenderer, &MAT_WHITE_SPECULAR);

	/* a vykreslime nas model (ve vychozim stavu kreslime pouze snimek 0) */
	//renderModel(pRenderer, pModel, 0);

	renderModel(pRenderer, pModel, snimok);
	fps();
	printf("%f\n", framespersecond);

}

/* Callback funkce volana pri tiknuti casovace
 * ticks - pocet milisekund od inicializace */
void onTimer(int ticks) {
	/* uprava parametru pouzivaneho pro vyber klicoveho snimku
	 * a pro interpolaci mezi snimky */
	snimok = (float)ticks/MILISEKUNDA;
}

/*****************************************************************************
 *****************************************************************************/
