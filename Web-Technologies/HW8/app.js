const request = require('request');
const express = require('express');
const path = require('path'); 
const http = require('http')

const axios = require('axios');
global.token = "eyJhbGciOiJIUzI1NiJ9.eyJyb2xlcyI6IiIsInN1YmplY3RfYXBwbGljYXRpb24iOiI2MjhlOTM2MTQzMjEwYjAwMGM1ZGZjYmQiLCJleHAiOjE2NTU0MDQ2MDEsImlhdCI6MTY1NDc5OTgwMSwiYXVkIjoiNjI4ZTkzNjE0MzIxMGIwMDBjNWRmY2JkIiwiaXNzIjoiR3Jhdml0eSIsImp0aSI6IjYyYTIzZGI5NzBmYzNmMDAwZDc3NzIzOCJ9.AyTxyX33LrZf0RrMhFSh5bkY148KGTP4AOoOi2pXQ7A";
const app = express(); 
var bodyParser = require("body-parser");
app.use(bodyParser.json());
app.use(express.urlencoded({ extended: true }));
app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    next();});
   
const port = parseInt(process.env.PORT) || 8000; 
app.use(express.static(__dirname + '/dist/hw8')); 
app.get('/', (req, res) => res.sendFile(path.join(__dirname+ '/dist/hw8/index.html'))); 
const server = http.createServer(app); 
server.listen(port, () => console.log(`App running on: http://localhost:${port}`));

app.get('/search/', (req, res) => {
    var name = req.query.artist;
    var url = "https://api.artsy.net/api/search?q="+name.toString();
    axios.get(url, {headers: {"X-XAPP-Token": global.token}}).then(response => {
        searchres = response.data._embedded.results;
        var result = [];
        for (var i=0; i < searchres.length; i++){
            var tmp = searchres[i];
            if (tmp.og_type == 'artist'){
                var imgurl = tmp._links.thumbnail.href
                if (imgurl == "/assets/shared/missing_image.png"){
                    imgurl = "0";
                }
                var id = tmp._links.self.href.split('/')
                tmp = {'title': tmp.title,
                        'image':imgurl,
                        'ID': id[id.length-1]};
                result.push(tmp);
            }
        }
        res.json({"result":result}).end();
     })
    .catch((error) => {
        console.log(error);
        res.json({"error": error}).end();
     });
});

app.get('/details/', (req, res) => {
    var id = req.query.artist_id;
    var url = "https://api.artsy.net/api/artists/"+id.toString();
    axios.get(url, {headers: {"X-XAPP-Token": global.token}}).then(response => {
        r = response.data;
        var bio = '';
        if (r.biography.length == 0) {
            bio += 'No biography. ';
        }
        if (r.deathday.length == 0) {
            bio += "No deathday. ";
        }
        if (r.birthday.length == 0) {
            bio += "No birthday. ";
        }
        if (r.nationality.length == 0) {
            bio += "No nationality. ";
        }
        res.json({'result':{'name':    r['name'], 
                    'birthday': r['birthday'], 
                    'deathday': r['deathday'], 
                    'nationality': r['nationality'],
                    'info':bio,
                    'biography': r['biography']}}).end();
     })
    .catch((error) => {
        res.json({"error": error}).end();
     });
});

app.get('/artworks/', (req, res) => {
    var id = req.query.artist_id;//"4d8b928b4eb68a1b2c0001f2";
    var url = "https://api.artsy.net/api/artworks?artist_id="+id.toString()+"&size=10";
    axios.get(url, {headers: {"X-XAPP-Token": global.token}}).then(response => {
        searchres = response.data._embedded.artworks;
        //res.json(searchres);
        var result = [];
        // [“id”] (ID of the artwork), [“title”] (name of the artwork), [“date”] (creation year of the artwork) and [“links”][“thumbnail”][“href”] 
        for (var i=0; i < searchres.length; i++){
            var tmp = searchres[i];
            var imgurl = tmp._links.thumbnail.href
            if (imgurl == "/assets/shared/missing_image.png"){
                imgurl = "./assets/logo.png";
            }
            tmp = {'ID': tmp.id,
                'title':tmp.title,
                'date':tmp.date,
                'image':imgurl};
            result.push(tmp);
        }
        res.json({'result':result}).end();
     })
    .catch((error) => {
        res.json({"error": error}).end();
     });
});

app.get('/genes/', (req, res) => {
    var id = req.query.artwork_id;"515b0f9338ad2d78ca000554";
    //https://api.artsy.net/api/genes?artwork_id=515b0f9338ad2d78ca000554
    var url = "https://api.artsy.net/api/genes?artwork_id="+id.toString();
    axios.get(url, {headers: {"X-XAPP-Token": global.token}}).then(response => {
        searchres = response.data._embedded.genes; 
        //res.json(searchres);
        var result = [];
        //[“name”] (name of the category) and [“_links”][“thumbnail”][“href”] 
        for (var i=0; i < searchres.length; i++){
            var tmp = searchres[i];
            var imgurl = tmp._links.thumbnail.href
            if (imgurl == "/assets/shared/missing_image.png"){
                imgurl = "./assets/logo.png";
            }
            tmp = {'name':tmp.name,
                'image':imgurl};
            result.push(tmp);
        }
        res.json({'result':result}).end();
     })
    .catch((error) => {
        res.json({"error": error}).end();
     });
});

module.exports = app;
