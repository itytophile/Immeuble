float x, y, taille, a, v, t=0, tg=0, vmax=11;
float[] xIm = new float[4], yIm = new float[4], tailleIm = new float[4];
Boolean inAir, saut, bloc=false, over = false;
int score = 0, fps = 60, R, G, B, scoreMOD;

void setup() {
  size(1000, 500);
  x=width/4;
  y=0;
  taille = 20;
  saut = false;
  frameRate(fps);
  xIm[0]=100;
  yIm[0]=200;
  tailleIm[0] = 300;
  xIm[1]=xIm[0]+tailleIm[0]+50+random(275);
  yIm[1]=200;
  tailleIm[1] = 300;
  xIm[3]=xIm[1]+tailleIm[1]+50+random(300);
  yIm[3]=200;
  tailleIm[3] = 300;
  xIm[2]=0;
  yIm[2]=0;
  tailleIm[2] = 0;
  ellipseMode(RADIUS);
}

void draw() {
  if (!over) {
    //On change la couleur du ciel
    noStroke();
    rectMode(CORNER);
    scoreMOD = score%200;
    if (scoreMOD<= 100) {
      R = 34*scoreMOD/25 - 83*scoreMOD*scoreMOD/2500 + 215; //Des fonctions du second degré pour contrôler les variations: jour > crépuscule > nuit
      G = 49*scoreMOD*scoreMOD/5000 - 287*scoreMOD/100 + 215;
      B = 59*scoreMOD*scoreMOD/1000 - 161*scoreMOD/20 + 255;
    }
    if (scoreMOD>=150) { //De la nuit vers le jour
      R = 4*scoreMOD-581;
      G = 27*scoreMOD/7 - 3868/7;
      B = 215*scoreMOD/49 - 30290/49;
    }
    fill(R, G, B);
    rect(0, 0, width, height);
    fill(255, 255, 0);
    ellipse(width/2, 4/5+height*scoreMOD/50 - 40, 40, 40);

    //le carré qu'on contrôle
    rectMode(CENTER);
    if (scoreMOD < 75 || scoreMOD > 175) fill(100);
    else {
      fill(255, 30);
      ellipse(x, y-taille/2, 60, 60);
      fill(254, 254, 226);
    }
    rect(x, y-taille/2, taille, taille);

    //On déssine les immeubles
    stroke(2);
    rectMode(CORNER);
    fill(200);
    for (int i =0; i < 4; i++) rect(xIm[i], yIm[i], tailleIm[i], height-yIm[i]);
    noStroke();
    if (scoreMOD >= 75 && scoreMOD <= 175) { //Le halo de lumière qui entoure le cube la nuit
      fill(255, 30);
      ellipse(x, y-taille/2, 60, 60);
    }


    //La condition qui vérifie si le carré tombe dans le vide
    if (y < yIm[0] || x > xIm[0]+tailleIm[0]+taille/2) inAir = true;

    //La condition qui vérifie si le cube a atterri sur le sol
    if (!bloc && inAir && y > yIm[0] && x < xIm[0]+tailleIm[0]+taille/2) {
      inAir = saut = false;
      y = yIm[0];
      tg = 0;
    }

    //La condition qui permet à la gravité d'agir

    if (inAir) y+=10*++tg*tg/720;

    //La condition qui detecte si la souris est à droite du cube pour avancer
    if (mouseX > x) a = 0.5;
    else t=0;

    //La condition qui vérifie si on a activé le saut
    if (saut) y-=10;

    //L'unité de temps pour déterminer une vitesse
    ++t;

    //Application du déplacement sur les immeubles
    for (int i =0; !bloc && i < 4; i++) 
      if (a*t*t/720<vmax) xIm[i]-=a*t*t/720;
      else xIm[i]-=vmax;

    //On change la propriété des immeubles à l'écran
    if (x>=xIm[1]+taille/2) {
      xIm[2]=xIm[0]; 
      yIm[2]=yIm[0]; 
      tailleIm[2]=tailleIm[0];
      xIm[0]=xIm[1]; 
      yIm[0]=yIm[1]; 
      tailleIm[0]=tailleIm[1];
      xIm[1]=xIm[3];
      yIm[1]=yIm[3];
      tailleIm[1]=tailleIm[3];
      xIm[3]=xIm[1]+tailleIm[1]+70+random(275);
      yIm[3]=150+random(100);
      tailleIm[3] = 200+random(100);
      if (y>yIm[0]) { //on vérifie si le cube se prend le mur de l'immeuble d'en face
        bloc = true;
        x=xIm[0]-taille/2;
      } else ++score;
    }
    //on vérifie si le cube tombe trop bas
    over = y>height*2;
    /**
     //debug
     fill(0);
     text(xIm[0]+tailleIm[0], 20, 20);
     text(x, 20, 40);
     text("xIm[2]="+xIm[2]+" yIm[2]="+yIm[2]+" tailleIm[2]="+tailleIm[2], 200, 20);
     **/
    if (scoreMOD < 75 || scoreMOD > 175) fill(0);
    else fill(255);
    /**
     text(R, width/2, 20);
     text(G, width/2, 40);
     text(B, width/2, 60);
     **/
    textSize(20);
    text(score, 20, 20);
    text("fps: "+frameRate, width-100, 20);
  } else {
    background(0);
    fill(255, 0, 0);
    textSize(32);
    textAlign(CENTER);
    text("lol", width/2, height/2);
    textSize(16);
    text("Score = "+score, width/2, height*3/4);
  }
}

void mousePressed() {
  if (!inAir) saut = inAir = true;
  else { //Pour forcer le cube à atterrir
    saut = false;
    y+=10;
  }
}