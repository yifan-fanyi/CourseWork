// cd '/home/alex/Desktop/hw8/' && ng build && node csci571_0603.js


function clearText() {
    location.reload();
}

function EnableDisable(txtPassportNumber) {
    var btnSubmit = document.getElementById("search-icon");
    if (txtPassportNumber.value.trim() != "") {
        btnSubmit.disabled = false;
    } else {
        btnSubmit.disabled = true;
    }
    var btnSubmit = document.getElementById("clear-icon");
    if (txtPassportNumber.value.trim() != "") {
        btnSubmit.disabled = false;
    } else {
        btnSubmit.disabled = true;
    }
};

function load_search() {
    document.getElementById("search-icon").onclick = function (event) {
        let artist = document.getElementById("searchtext").value
        if (artist.length > 0){
        load_Artists();}
        
    };
}

function show_Artist(artist) {
    let img = document.createElement("img");
    img.classList.add("card-img-top");
    img.alt = "Card image cap";
    img.src = artist.image;
                    
    let name_div = document.createElement("div");
    name_div.classList.add("card-body");
    let n = document.createElement("p");
    n.classList.add('card-title');
    n.classList.add('text-center');
    n.textContent = artist.title;
    name_div.appendChild(n);

    let result_div = document.createElement("div");
    result_div.classList.add("card");
    result_div.classList.add("text-white");
    result_div.classList.add("bg-primary1");
    result_div.style.width = "100%";
    result_div.style.height = "100%";
    result_div.appendChild(img);
    result_div.appendChild(name_div);
    let result_divs = document.createElement("div");
    result_divs.classList.add("col-3");
    result_divs.appendChild(result_div);
    return result_divs;
}

function show_Artists(artists) {
    let tmp = document.getElementById("search-results");
    let results_div = document.createElement("div");
    if (artists.length === 0) {
        let no_results = document.createElement("p");
        no_results.classList.add("alert");
        no_results.classList.add("alert-danger");
        no_results.appendChild(document.createTextNode("No results found."));
        results_div.appendChild(no_results);

        let spinner = document.getElementById("spinner");
        spinner.style.visibility = 'hidden';
        while (tmp.firstChild) {
            tmp.firstChild.remove();
        }
        for (i=0; i<results_div.children.length;i++) {
            tmp.appendChild(results_div.children[i]);
        }
        return;
    }
    for (let i = 0; i < artists.length; i++) {
        let one_result = show_Artist(artists[i]);
        one_result.onclick = function () {
            load_Detail(artists[i].ID);
            load_artworks(artists[i].ID);
            one_result.style.backgroundColor = "#0e344e!important";
            one_result.classList.remove('bg-primary1');
            one_result.classList.add('active');
        };
        results_div.appendChild(one_result);
    }
    let spinner = document.getElementById("spinner");
    spinner.style.visibility = 'hidden';
    while (tmp.firstChild) {
        tmp.firstChild.remove();
    }
    for (i=0; i<results_div.children.length;i++) {
        tmp.appendChild(results_div.children[i]);
    }
let details = document.getElementById("details");
    details.style.visibility = 'hidden';
    let details1 = document.getElementById("details1");
    details1.style.visibility = 'hidden';
}

function load_Artists() {
    let spinner = document.getElementById("spinner");
    spinner.style.visibility = 'visible';
    
    let artist = document.getElementById("searchtext").value;
    fetch(`/search/?artist=${artist}`, {method: 'GET', headers:{'Content-Type':'application/json'}})
        .then(response => response.json())
        .then(data => show_Artists(data.result));
}

function load_Detail(artist_id) {
    let spinner = document.getElementById("spinner1");
    spinner.style.visibility = 'visible';
    fetch(`/details/?artist_id=${artist_id}`)
        .then(response => response.json())
        .then(data => show_Detail(data.result));
}

function show_Detail(artist) {
    let tmp = document.getElementById("search-details");
    let result_div = document.createElement("div");
    let aname_div = document.createElement("div");
    aname_div.classList.add("aname_div");
    let n = document.createElement("p");
    n.textContent = artist.name + "   ("  +artist.birthday +'-'+artist.deathday +")";
    aname_div.appendChild(n);
    result_div.appendChild(aname_div);

    let nation_div = document.createElement("div");
    nation_div.classList.add("nation_div");
    let nation = document.createElement("p");
    nation.textContent = artist.nationality;
    nation_div.appendChild(nation);
    result_div.appendChild(nation_div);

    let info_div = document.createElement("div");
    info_div.classList.add("info_div");
    let info = document.createElement("p");
    info.textContent = artist.info;
    info_div.appendChild(info);
    result_div.appendChild(info_div);

    let bio_div = document.createElement("div");
    bio_div.classList.add("bio_div");
    let bio = document.createElement("p");
    bio.textContent = artist.biography;
    bio_div.appendChild(bio);
    result_div.appendChild(bio_div);

    let spinner = document.getElementById("spinner1");
    spinner.style.visibility = 'hidden';
    let details = document.getElementById("details");
    details.style.visibility = 'visible';
    let details1 = document.getElementById("details1");
    details1.style.visibility = 'visible';


    while (tmp.firstChild) {
        tmp.firstChild.remove();
    }
    for (i=0; i<result_div.children.length;i++) {
        tmp.appendChild(result_div.children[i]);
    }
}

function show_artwork(artwork) {
    let img = document.createElement("img");
    img.classList.add("card-img-top");
    img.alt = "Card image cap";
    img.src = artwork.image;

    let body = document.createElement('div');
    body.classList.add("card-body")  ;        
    let n = document.createElement("p");
    n.classList.add('card-title');
    n.classList.add('text-center');
    n.textContent = artwork.title;

    let nn = document.createElement("p");
    nn.classList.add('card-text');
    nn.classList.add('text-center');
    nn.textContent = artwork.date;
    body.appendChild(n);
    body.appendChild(nn);

    let bt = document.createElement("a");
    bt.classList.add("btn");
    bt.classList.add("btn-sm");
    bt.classList.add("btn-primary");
    bt.classList.add("ml-3");
    bt.textContent = "Categories";
    let result_div = document.createElement("div");
    result_div.classList.add("card");
    result_div.classList.add("bg-light");
    result_div.classList.add("mb-3");

    result_div.style.width = "100%";
    result_div.style.height = "100%";
    result_div.appendChild(img);
    result_div.appendChild(body);
    result_div.appendChild(bt);
    let result_divs = document.createElement("div");
    result_divs.classList.add("col-3");
    result_divs.appendChild(result_div);
    return result_divs;
}

function show_artworks(artworks) {
    let tmp = document.getElementById("search-artworks1");
    let results_div = document.createElement("div");
    if (artworks.length === 0) {
        let no_results = document.createElement("p");
        no_results.classList.add("alert");
        no_results.classList.add("alert-danger");
        no_results.appendChild(document.createTextNode("No artworks."));
        results_div.appendChild(no_results);

        let spinner = document.getElementById("spinner");
        spinner.style.visibility = 'hidden';
        while (tmp.firstChild) {
            tmp.firstChild.remove();
        }
        for (i=0; i<results_div.children.length;i++) {
            tmp.appendChild(results_div.children[i]);
        }
        return;
    }
    for (let i = 0; i < artworks.length; i++) {
        let one_result = show_artwork(artworks[i]);
        one_result.onclick = function () {
            load_genus(artworks[i]);
        };
        results_div.appendChild(one_result);
    }
    while (tmp.firstChild) {
        tmp.firstChild.remove();
    }
    for (i=0; i<results_div.children.length;i++) {
        tmp.appendChild(results_div.children[i]);
    }
}

function load_artworks(artist_id) {
    let results_div = document.getElementById("search-artworks1");
    while (results_div.firstChild) {
        results_div.firstChild.remove();
    }
    fetch(`/artworks/?artist_id=${artist_id}`, {method: 'GET', headers:{'Content-Type':'application/json'}})
        .then(response => response.json())
        .then(data => show_artworks(data.result));
}

function load_genus(artwork){
    document.querySelector(".modal").classList.add("show"); 
    document.querySelector(".modal").style.display = "block";
    let spinner = document.getElementById("spinner2");
    spinner.style.visibility = 'visible';

    let img = document.createElement("img");
    img.classList.add("border-0");
    img.style.width = "20%";
    img.style.height = "20%";
    img.alt = "Card image cap";
    img.src = artwork.image;

    let body = document.createElement('div');
    body.classList.add("card-block")  ;  
    body.classList.add("px-2")  ;        
    let n = document.createElement("p");
    n.classList.add('card-title');
    n.classList.add('text-center');
    n.classList.add('h6');
    n.textContent = artwork.title;

    let nn = document.createElement("p");
    nn.classList.add('card-text');
    nn.classList.add('text-center');
    nn.classList.add('h6');
    nn.textContent = artwork.date;
    body.appendChild(n);
    body.appendChild(nn);

    let result_div = document.createElement("div");
    result_div.classList.add("card");
    result_div.classList.add("bg-light");
    result_div.classList.add("flex-row");
    result_div.style.width = "100%";
    result_div.style.height = "100%";
    result_div.appendChild(img);
    result_div.appendChild(body);

    let c = document.getElementById("dialog_img");
    while (c.firstChild) {
        c.firstChild.remove();
    }
    c.appendChild(result_div);

    fetch(`/genes/?artwork_id=${artwork.ID}`, {method: 'GET', headers:{'Content-Type':'application/json'}})
        .then(response => response.json())
        .then(data => show_genes(data.result));

}

function show_genes(genes){
    let results_div = document.getElementById("modal-body");
    while (results_div.firstChild) {
        results_div.firstChild.remove();
    }
    if (genes.length === 0) {
        let no_results = document.createElement("p");
        no_results.classList.add("alert");
        no_results.classList.add("alert-danger");
        no_results.appendChild(document.createTextNode("No genes."));
        results_div.appendChild(no_results);

        let spinner = document.getElementById("spinner2");
        spinner.style.visibility = 'hidden';
        return;
    }
    for (let i = 0; i < genes.length; i++) {
        let one_result = show_gene(genes[i]);
        results_div.appendChild(one_result);
    }
    let spinner = document.getElementById("spinner2");
    spinner.style.visibility = 'hidden';
}

function show_gene(gene){
    let img = document.createElement("img");
    img.classList.add("card-img-top");
    img.alt = "Card image cap";
    img.src = gene.image;

    let body = document.createElement('div');
    body.classList.add("card-body")  ;        
    let n = document.createElement("p");
    n.classList.add('card-title');
    n.classList.add('text-center');
    n.textContent = gene.name;
    body.appendChild(n);

    let result_div = document.createElement("div");
    result_div.classList.add("card");
    result_div.classList.add("bg-light");
    result_div.appendChild(img);
    result_div.appendChild(body);
    let result_divs = document.createElement("div");
    result_divs.classList.add("col-3");
    result_divs.appendChild(result_div);
    return result_divs;
}

function closemodal(){
    location.reload();
}
window.onload = load_search;
var slide_index = 0;
