#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <stack>

#include "libTraitement.h"
//copy /y "D:\Alexis\Documents\Visual Studio 2015\Projects\Projet_vision\Traitement\fftw\libfftw3 - 3.dll" "D:\Alexis\Documents\Visual Studio 2015\Projects\Projet_vision\seuilAuto\bin\Debug\"


ClibTraitement::ClibTraitement() {

	this->nbDataImg = 0;
	this->dataFromImg.clear();
	this->imgPt = NULL;
}

// M�thode de traitement par d�faut -> constructeur par d�faut via le C#
ClibTraitement::ClibTraitement(int nbChamps, byte* data, int stride, int nbLig, int nbCol){

	// affectation des signatures (dataFromImg) et taille (nbChamps)
	this->nbDataImg = nbChamps;
	this->dataFromImg.resize(nbChamps);

	this->imgPt = new CImageCouleur(nbLig, nbCol);
	CImageCouleur out(nbLig, nbCol);

	// byte* data est la donn�e des images

	byte* pixPtr = (byte*)data;

	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)	//rvb
		{
			this->imgPt->operator()(y, x)[0] = pixPtr[3 * x + 2];
			this->imgPt->operator()(y, x)[1] = pixPtr[3 * x + 1];
			this->imgPt->operator()(y, x)[2] = pixPtr[3 * x ];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}
	
	//CImageNdg imgsauv;
	//imgsauv = this->imgPt->plan();
	//imgsauv.sauvegarde("test")



	/*********************************************************************************************************************

	DEBUT DE L'ALGORITHME DE TRAITEMENT

	**********************************************************************************************************************/

	CImageNdg seuil;

	int seuilBas = 128;
	int seuilHaut = 255;

	seuil = this->imgPt->plan().seuillage("automatique",seuilBas,seuilHaut);

	this->dataFromImg.at(0) = seuilBas;

	for (int i = 0; i < seuil.lireNbPixels(); i++)
	{
		out(i)[0] = (unsigned char)(255*(int)seuil(i));
		out(i)[1] = 0;
		out(i)[2] = 0;
	}
		
	/*********************************************************************************************************************

	FIN DU TRAITEMENT,
	AFFICHAGE RETOUR VERS LE C#

	**********************************************************************************************************************/

	pixPtr = (byte*)data;
	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)
		{
			pixPtr[3 * x + 2] = out(y, x)[0];
			pixPtr[3 * x + 1] = out(y, x)[1];
			pixPtr[3 * x] = out(y, x)[2];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}
}

ClibTraitement::ClibTraitement(byte* datain, int stride, int nbLig, int nbCol, int seuilB, int seuilH) {

	this->nbDataImg = 0;
	this->dataFromImg.resize(0);

	CImageCouleur in(nbLig, nbCol);

	// byte* data est la donn�e des images

	byte* pixPtr = (byte*)datain;

	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)	//rvb
		{
			in(y, x)[0] = pixPtr[3 * x + 2];
			in(y, x)[1] = pixPtr[3 * x + 1];
			in(y, x)[2] = pixPtr[3 * x];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}

	//CImageNdg imgsauv;
	//imgsauv = this->imgPt->plan();
	//imgsauv.sauvegarde("test")



	/*********************************************************************************************************************

	DEBUT DE L'ALGORITHME DE TRAITEMENT

	**********************************************************************************************************************/

	this->imgPt = new CImageCouleur(in.rognageSigComposante("manuel", seuilB, seuilH, 0, "non"));

	/*********************************************************************************************************************

	FIN DU TRAITEMENT,
	AFFICHAGE RETOUR VERS LE C#

	**********************************************************************************************************************/

	//pixPtr = (byte*)dataout;
	//for (int y = 0; y < this->imgData()->lireHauteur(); y++)
	//{
	//	for (int x = 0; x < this->imgData()->lireLargeur(); x++)
	//	{
	//		pixPtr[3 * x + 2] = this->imgData()->operator()(y, x)[0];
	//		pixPtr[3 * x + 1] = this->imgData()->operator()(y, x)[1];
	//		pixPtr[3 * x] = this->imgData()->operator()(y, x)[2];
	//	}
	//	pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	//}
}

ClibTraitement::~ClibTraitement() {
	
	if (imgPt)
		(*this->imgPt).~CImageCouleur(); 
	this->dataFromImg.clear();
}

void ClibTraitement::Traitement2(int nbChamps, byte * data, int stride, int nbLig, int nbCol, double parametres[10], int nbChamps_p, byte * data_p, int stride_p, int nbLig_p, int nbCol_p)
{
	// --------------------------- Construction de l'image ClibTraitement ----------------------------------
	// affectation des signatures (dataFromImg) et taille (nbChamps)
	this->nbDataImg = nbChamps;
	this->dataFromImg.resize(nbChamps);

	this->imgPt = new CImageCouleur(nbLig, nbCol);
	CImageCouleur out(nbLig, nbCol);
	// byte* data est la donn�e des images
	byte* pixPtr = (byte*)data;

	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)	//rvb
		{
			this->imgPt->operator()(y, x)[0] = pixPtr[3 * x + 2];
			this->imgPt->operator()(y, x)[1] = pixPtr[3 * x + 1];
			this->imgPt->operator()(y, x)[2] = pixPtr[3 * x];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}

	// Cr�ation d'une image pour la piece envoyee
	/*
	ClibTraitement piece;
	piece.nbDataImg = nbChamps_p;
	piece.dataFromImg.resize(nbChamps_p);
	piece.imgPt = new CImageCouleur(nbLig_p, nbCol_p);
	byte* pixPtr_p = (byte*)data_p;
	for (int y = 0; y < nbLig_p; y++)
	{
		for (int x = 0; x < nbCol_p; x++)
		{
			piece.imgPt->operator()(y, x)[0] = pixPtr_p[3 * x + 2];
			piece.imgPt->operator()(y, x)[1] = pixPtr_p[3 * x + 1];
			piece.imgPt->operator()(y, x)[2] = pixPtr_p[3 * x];
		}
		pixPtr_p += stride_p;
	}
	*/
	
	CImageCouleur piece2(nbLig_p, nbCol_p);
	byte* pixPtr_p2 = (byte*)data_p;
	for (int y = 0; y < nbLig_p; y++)
	{
		for (int x = 0; x < nbCol_p; x++)
		{
			piece2(y, x)[0] = pixPtr_p2[3 * x + 2];
			piece2(y, x)[1] = pixPtr_p2[3 * x + 1];
			piece2(y, x)[2] = pixPtr_p2[3 * x];
		}
		pixPtr_p2 += stride_p;
	}


	/*********************************************************************************************************************

	DEBUT DE L'ALGORITHME DE TRAITEMENT

	**********************************************************************************************************************/

	// Traitement sur image niveaux de gris qui �tait pr�sent par le pass�, simple filtrage

	//CImageNdg moyen;
	//moyen = this->imgPt->plan().filtrage("moyennage", (int)parametres[0], (int)parametres[1]);
	//this->dataFromImg.at(0) = parametres[0];
	//for (int i = 0; i < moyen.lireNbPixels(); i++)
	//{
	//	out(i)[0] = (unsigned char)(255 * (int)moyen(i));
	//	out(i)[1] = (unsigned char)(255 * (int)moyen(i));
	//	out(i)[2] = (unsigned char)(255 * (int)moyen(i));
	//}


	// Appel � la fonction detection piece qui a partir de detection_ref et piece2 renvoie une image du puzzle avec un cadre rouge au niveau de la d�tection
	CImageCouleur detection_ref; // d�claration de l'image qui sera renvoy�e par notre fonction de d�tection
	double score = -1; // d�claration d'une valeur de score de similarit� qui sera modifi�e par pointeur
	double score2 = -1; // score de la deuxi�me image la plus proche
	detection_ref = this->imgPt->detection_piece(piece2, &score, &score2);
	this->dataFromImg.at(0) = floor(exp(3*(0-score))*100); // �criture du score
	this->dataFromImg.at(1) = floor(exp(3 * (0 - score2)) * 100);

	// Copie de l'image r�sultante dans out pour renvoyer au C#
	for (int i = 0; i < detection_ref.lireNbPixels(); i++)
	{
		out(i)[0] = detection_ref(i)[0];
		out(i)[1] = detection_ref(i)[1];
		out(i)[2] = detection_ref(i)[2];
	}

	/*********************************************************************************************************************

	FIN DU TRAITEMENT,
	AFFICHAGE RETOUR VERS LE C#

	**********************************************************************************************************************/

	pixPtr = (byte*)data;
	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)
		{
			pixPtr[3 * x + 2] = out(y, x)[0];
			pixPtr[3 * x + 1] = out(y, x)[1];
			pixPtr[3 * x] = out(y, x)[2];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}
}

void ClibTraitement::TraitementRognage(int nbChamps, byte * data, int stride, int nbLig, int nbCol, double parametres[10], int nbChamps_p, byte * data_p, int stride_p, int nbLig_p, int nbCol_p)
{
	// --------------------------- Construction de l'image ClibTraitement ----------------------------------
	// affectation des signatures (dataFromImg) et taille (nbChamps)
	this->nbDataImg = nbChamps;
	this->dataFromImg.resize(nbChamps);

	this->imgPt = new CImageCouleur(nbLig, nbCol);
	CImageCouleur out(nbLig, nbCol);
	// byte* data est la donn�e des images
	byte* pixPtr = (byte*)data;

	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)	//rvb
		{
			this->imgPt->operator()(y, x)[0] = pixPtr[3 * x + 2];
			this->imgPt->operator()(y, x)[1] = pixPtr[3 * x + 1];
			this->imgPt->operator()(y, x)[2] = pixPtr[3 * x];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}

	// Cr�ation d'une image pour la piece envoyee
	/*
	ClibTraitement piece;
	piece.nbDataImg = nbChamps_p;
	piece.dataFromImg.resize(nbChamps_p);
	piece.imgPt = new CImageCouleur(nbLig_p, nbCol_p);
	byte* pixPtr_p = (byte*)data_p;
	for (int y = 0; y < nbLig_p; y++)
	{
	for (int x = 0; x < nbCol_p; x++)
	{
	piece.imgPt->operator()(y, x)[0] = pixPtr_p[3 * x + 2];
	piece.imgPt->operator()(y, x)[1] = pixPtr_p[3 * x + 1];
	piece.imgPt->operator()(y, x)[2] = pixPtr_p[3 * x];
	}
	pixPtr_p += stride_p;
	}
	*/


	CImageCouleur piece2(nbLig_p, nbCol_p);
	byte* pixPtr_p2 = (byte*)data_p;
	for (int y = 0; y < nbLig_p; y++)
	{
		for (int x = 0; x < nbCol_p; x++)
		{
			piece2(y, x)[0] = pixPtr_p2[3 * x + 2];
			piece2(y, x)[1] = pixPtr_p2[3 * x + 1];
			piece2(y, x)[2] = pixPtr_p2[3 * x];
		}
		pixPtr_p2 += stride_p;
	}

	CImageCouleur piece_rognee = piece2.rognageSigComposante("manuel", 80, 255, 2, "HSV");
	

	/*********************************************************************************************************************

	DEBUT DE L'ALGORITHME DE TRAITEMENT

	**********************************************************************************************************************/

	// Traitement sur image niveaux de gris qui �tait pr�sent par le pass�, simple filtrage

	//CImageNdg moyen;
	//moyen = this->imgPt->plan().filtrage("moyennage", (int)parametres[0], (int)parametres[1]);
	//this->dataFromImg.at(0) = parametres[0];
	//for (int i = 0; i < moyen.lireNbPixels(); i++)
	//{
	//	out(i)[0] = (unsigned char)(255 * (int)moyen(i));
	//	out(i)[1] = (unsigned char)(255 * (int)moyen(i));
	//	out(i)[2] = (unsigned char)(255 * (int)moyen(i));
	//}


	// Appel � la fonction detection piece qui a partir de detection_ref et piece2 renvoie une image du puzzle avec un cadre rouge au niveau de la d�tection
	CImageCouleur detection_ref; // d�claration de l'image qui sera renvoy�e par notre fonction de d�tection
	double score = -1; // d�claration d'une valeur de score de similarit� qui sera modifi�e par pointeur
	double score2 = -1; // score de la deuxi�me image la plus proche

	//detection_ref = this->imgPt->detection_piece(piece2, &score, &score2);
	detection_ref = this->imgPt->detection_piece(piece_rognee, &score, &score2);
	this->dataFromImg.at(0) = floor(exp(3 * (0 - score)) * 100); // �criture du score
	this->dataFromImg.at(1) = floor(exp(3 * (0 - score2)) * 100);

	// Copie de l'image r�sultante dans out pour renvoyer au C#
	for (int i = 0; i < detection_ref.lireNbPixels(); i++)
	{
		out(i)[0] = detection_ref(i)[0];
		out(i)[1] = detection_ref(i)[1];
		out(i)[2] = detection_ref(i)[2];
	}

	/*********************************************************************************************************************

	FIN DU TRAITEMENT,
	AFFICHAGE RETOUR VERS LE C#

	**********************************************************************************************************************/

	pixPtr = (byte*)data;
	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)
		{
			pixPtr[3 * x + 2] = out(y, x)[0];
			pixPtr[3 * x + 1] = out(y, x)[1];
			pixPtr[3 * x] = out(y, x)[2];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}

	pixPtr_p2 = (byte*)data_p;
	for (int y = 0; y < nbLig_p; y++)
	{
		for (int x = 0; x < nbCol_p; x++)
		{
			pixPtr_p2[3 * x + 2] = piece2(y, x)[0];
			pixPtr_p2[3 * x + 1] = piece2(y, x)[1];
			pixPtr_p2[3 * x] = piece2(y, x)[2];
		}
		pixPtr_p2 += stride_p;
	}
	
}

void ClibTraitement::copydata(byte* data, int stride)
{
	byte* pixPtr = (byte*)data;
	for (int y = 0; y < this->imgData()->lireHauteur(); y++)
	{
		for (int x = 0; x < this->imgData()->lireLargeur(); x++)	//rvb
		{
			pixPtr[3 * x + 2] = this->imgData()->operator()(y, x)[0];
			pixPtr[3 * x + 1] = this->imgData()->operator()(y, x)[1];
			pixPtr[3 * x] = this->imgData()->operator()(y, x)[2];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}
}

void ClibTraitement::TraitementMatching(byte * data, int stride, int nbLig, int nbCol, byte * data_p, int stride_p, int nbLig_p, int nbCol_p)
{
	// --------------------------- Construction de l'image ClibTraitement ----------------------------------
	// affectation des signatures (dataFromImg) et taille (nbChamps)
	this->nbDataImg = 2;
	this->dataFromImg.resize(this->nbDataImg);

	this->imgPt = new CImageCouleur(nbLig, nbCol);
	CImageCouleur out(nbLig, nbCol);
	// byte* data est la donn�e des images
	byte* pixPtr = (byte*)data;

	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)	//rvb
		{
			this->imgPt->operator()(y, x)[0] = pixPtr[3 * x + 2];
			this->imgPt->operator()(y, x)[1] = pixPtr[3 * x + 1];
			this->imgPt->operator()(y, x)[2] = pixPtr[3 * x];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}

	CImageCouleur pattern(nbLig_p, nbCol_p);
	byte* pixPtr_p = (byte*)data_p;
	for (int y = 0; y < nbLig_p; y++)
	{
		for (int x = 0; x < nbCol_p; x++)
		{
			pattern(y, x)[0] = pixPtr_p[3 * x + 2];
			pattern(y, x)[1] = pixPtr_p[3 * x + 1];
			pattern(y, x)[2] = pixPtr_p[3 * x];
		}
		pixPtr_p += stride_p;
	}


	/*********************************************************************************************************************

	DEBUT DE L'ALGORITHME DE TRAITEMENT

	**********************************************************************************************************************/

	CImageDouble img_scene(this->imgPt->plan(), "cast");
	CImageDouble img_pattern(pattern.plan(), "cast");

	CImageDouble img_corr = img_pattern.NormCorr(img_scene);
	int n = 0, i, j;
	// n = std:: find(0,img_corr.lireNbPixels(),img_corr.lireMax()); // ne marche pas :c
	while ((img_corr(n) != img_corr.lireMax()) && (n < img_corr.lireNbPixels()))
		n++;
	i = n / img_corr.lireLargeur() - img_pattern.lireHauteur() / 2;
	j = n % img_corr.lireLargeur() - img_pattern.lireLargeur() / 2;
	
	// renvoi des coordon�es trouv�es dans le C#
	this->dataFromImg.at(0) = i;
	this->dataFromImg.at(1) = j;

	//// Copie de l'image r�sultante dans out pour renvoyer au C#
	//for (int i = 0; i < img_corr.lireNbPixels(); i++)
	//{
	//	out(i)[0] = detection_ref(i)[0];
	//	out(i)[1] = detection_ref(i)[1];
	//	out(i)[2] = detection_ref(i)[2];
	//}


	/*********************************************************************************************************************

	FIN DU TRAITEMENT,
	AFFICHAGE RETOUR VERS LE C#

	**********************************************************************************************************************/

	//pas de modifs donc pas besoin de modifier l'image � nouveau

	//pixPtr = (byte*)data;
	//for (int y = 0; y < nbLig; y++)
	//{
	//	for (int x = 0; x < nbCol; x++)
	//	{
	//		pixPtr[3 * x + 2] = out(y, x)[0];
	//		pixPtr[3 * x + 1] = out(y, x)[1];
	//		pixPtr[3 * x] = out(y, x)[2];
	//	}
	//	pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	//}
}
