const data = {
  "website": "Decora",
  "categories": [
    {
      "name": "Dnevna soba",
      "image": "dnevnasoba.jpg",
      "products": [
        { "name": "Sofa", "image": "sofa.jpg", "price": 329.99 },
        { "name": "TV stol", "image": "tvstol.jpg", "price": 159.99 },
        { "name": "Rasvjeta", "image": "rasvjeta.jpg", "price": 324.99 },
        { "name": "Stolić", "image": "stolic.jpg", "price": 79.99 },
        { "name": "Fotelja", "image": "fotelja.jpg", "price": 104.99 }
      ]
    },
    {
      "name": "Spavaća soba",
      "image": "spavacasoba.jpg",
      "products": [
        { "name": "Krevet", "image": "krevet.jpg", "price": 239.99 },
        { "name": "Noćni ormarić", "image": "nocniormaric.jpg", "price": 59.99 },
        { "name": "Ormar", "image": "ormar.jpg", "price": 174.99 },
        { "name": "Komoda", "image": "komoda.jpg", "price": 119.99 },
        { "name": "Tepih", "image": "tepih.jpg", "price": 69.99 }
      ]
    },
    {
      "name": "Kuhinja",
      "image": "kuhinja.jpg",
      "products": [
        { "name": "Kuhinjski stol", "image": "kuhinjskistol.jpg", "price": 144.99 },
        { "name": "Kuhinjska stolica", "image": "kuhinjskastolica.jpg", "price": 39.99 },
        { "name": "Regal za kuhinju", "image": "regalkuhinja.jpg", "price": 109.99 },
        { "name": "Otok sa sjedalima", "image": "otok.jpg", "price": 349.99 },
        { "name": "Kuhinjski pribor", "image": "pribor.jpg", "price": 49.99 }
      ]
    },
    {
      "name": "Ured",
      "image": "ured.jpg",
      "products": [
        { "name": "Uredska stolica", "image": "uredskastolica.jpg", "price": 89.99 },
        { "name": "Radni stol", "image": "radnistol.jpg", "price": 134.99 },
        { "name": "Stalak za laptop", "image": "stalakzalaptop.jpg", "price": 19.99 },
        { "name": "Police", "image": "police.jpg", "price": 59.99 },
        { "name": "Lampa za radni stol", "image": "lamparadnistol.jpg", "price": 34.99 }
      ]
    },
    {
      "name": "Dekoracije",
      "image": "dekoracije.jpg",
      "products": [
        { "name": "Slika", "image": "slika.jpg", "price": 19.99 },
        { "name": "Vaza", "image": "vaza.jpg", "price": 14.99 },
        { "name": "Male fotografije", "image": "malefotografije.jpg", "price": 39.99 },
        { "name": "Viseće žarulje", "image": "visecezarulje.jpg", "price": 9.99 },
        { "name": "Cvijeće", "image": "cvijece.jpg", "price": 14.99 }
      ]
    },
    {
      "name": "Igraonica",
      "image": "igraonica.jpg",
      "products": [
        { "name": "Gaming stolica", "image": "gamingstolica.jpg", "price": 174.99 },
        { "name": "Gaming stol", "image": "gamingstol.jpg", "price": 199.99 },
        { "name": "Mehanička tipkovnica", "image": "mehanickatipkovnica.jpg", "price": 59.99 },
        { "name": "Monitor", "image": "monitor.jpg", "price": 49.99 },
        { "name": "Digitalna lampa", "image": "digitalnalampa.jpg", "price": 19.99 }
      ]
    },
    {
      "name": "Dječja soba",
      "image": "djecjasoba.jpg",
      "products": [
        { "name": "Krevet za djecu", "image": "krevetdijete.jpg", "price": 114.99 },
        { "name": "Ormarić za igračke", "image": "ormariczaigracke.jpg", "price": 39.99 },
        { "name": "Radni stol za djecu", "image": "radnistolzaidjecu.jpg", "price": 59.99 },
        { "name": "Plišane igračke", "image": "plisanci.jpg", "price": 4.99 },
        { "name": "Priobor za kreativnost", "image": "kreativnost.jpg", "price": 4.49 }
      ]
    },
    {
      "name": "Vrt",
      "image": "vrt.jpg",
      "products": [
        { "name": "Vrtna garnitura", "image": "vrtnagarnitura.jpg", "price": 234.99 },
        { "name": "Ležaljka", "image": "lezaljka.jpg", "price": 79.99 },
        { "name": "Stolić za vrt", "image": "stoliczavrt.jpg", "price": 19.99 },
        { "name": "Svjetiljka", "image": "svjetiljka.jpg", "price": 49.99 },
        { "name": "Suncobran", "image": "suncobran.jpg", "price": 69.99 }
      ]
    },
    {
      "name": "Garaža",
      "image": "garaza.jpg",
      "products": [
        { "name": "Police za alat", "image": "policezaalat.jpg", "price": 34.99 },
        { "name": "Alat", "image": "alat.jpg", "price": 39.99 },
        { "name": "Kutija za alate", "image": "kutijazaalat.jpg", "price": 34.99 },
        { "name": "Slike za garažu", "image": "slikezagarazu.jpg", "price": 19.99 },
        { "name": "Kante za boju", "image": "kantezaboju.jpg", "price": 34.99 }
      ]
    },
    {
      "name": "Kupaonica",
      "image": "kupaonica.jpg",
      "products": [
        { "name": "Ormarići za kupaonicu", "image": "ormaricikupaonica.jpg", "price": 84.99 },
        { "name": "Kada", "image": "kada.jpg", "price": 119.99 },
        { "name": "Staklo za tuš", "image": "staklozatus.jpg", "price": 59.99 },
        { "name": "Ogledalo sa svjetlom", "image": "ogledalosasvjetlom.jpg", "price": 49.99 },
        { "name": "Ručnici", "image": "rucnici.jpg", "price": 4.99 }
      ]
    }
  ]
};

module.exports = data;
