function clearText() {
    document.getElementById("searchtext").value = "Please enter an artist name";
}

function load_search() {
    document.getElementById("search-icon").onclick = function (event) {
        event.preventDefault();
        if (document.getElementById("wrapper").reportValidity()) {
            load_Artists();
        }
    };
}

function show_Artist(artist) {
    let art_div = document.createElement("div");
    art_div.classList.add("art_div");
    let img = document.createElement("img");
    img.src = artist.image;
    art_div.appendChild(img);
                    
    let name_div = document.createElement("div");
    name_div.classList.add("name_div");
    let n = document.createElement("p");
    n.textContent = artist.title
    name_div.appendChild(n);

    let result_div = document.createElement("div");
    result_div.classList.add("result_div")
    result_div.appendChild(art_div);

    result_div.appendChild(name_div);

    let result_divs = document.createElement("div");
    result_divs.classList.add("result_divs")
    result_divs.appendChild(result_div);

    return result_divs;
}

function show_Artists(artists) {
    let results_div = document.getElementById("search-results");
    while (results_div.firstChild) {
        results_div.firstChild.remove();
    }
    if (artists.length === 0) {
        let no_results = document.createElement("p");
        no_results.classList.add("no-results");
        no_results.appendChild(document.createTextNode("No results found."));
        results_div.appendChild(no_results);

        let loading = document.getElementById("loading");
        while (loading.firstChild) {
            loading.firstChild.remove();
        }
        return;
    }
    for (let i = 0; i < artists.length; i++) {
        let one_result = show_Artist(artists[i]);
        one_result.onclick = function () {
            load_Detail(artists[i].ID);
        };
        results_div.appendChild(one_result);
    }
    let loading = document.getElementById("loading");
    while (loading.firstChild) {
        loading.firstChild.remove();
    }
}

function load_Artists() {
    let result_div = document.getElementById("search-details");
    while (result_div.firstChild) {
        result_div.firstChild.remove();
    }   
    let results_div = document.getElementById("search-results");
    while (results_div.firstChild) {
        results_div.firstChild.remove();
    }
    let loading = document.getElementById("loading");
    let loadimg = document.createElement("img");
    loadimg.src = "./static/loading.gif";
    loading.appendChild(loadimg);
    let artist = document.getElementById("searchtext").value;
    fetch(`/search/?artist=${artist}`).then(
        function(response) {
            console.log("artist");
            if (response.status !== 200) {
                console.log(response);
                return;
            }
            response.json()
                .then(data => {
                    show_Artists(data.result);
                });
        }).catch(err => {
        console.log(err);
    });
}

function load_Detail(artist_id) {
    let result_div = document.getElementById("search-details");
    while (result_div.firstChild) {
        result_div.firstChild.remove();
    }   
    let loading = document.getElementById("loading1");
    let loadimg = document.createElement("img");
    loadimg.src = "./static/loading.gif";
    loading.appendChild(loadimg);
    fetch(`/detail/?artist_id=${artist_id}`).then(
        function(response) {
            console.log("artist");
            if (response.status !== 200) {
                console.log(response);
                return;
            }
            response.json()
                .then(data => {
                    show_Detail(data.result);
                });
        }
        ).catch(err => {
        console.log(err);
    });
}

function show_Detail(artist) {
    let result_div = document.getElementById("search-details");
    while (result_div.firstChild) {
        result_div.firstChild.remove();
    }    
    let aname_div = document.createElement("div");
    aname_div.classList.add("aname_div");
    let n = document.createElement("p");
    n.textContent = artist.name + "   ("  +artist.birthday +'-'+artist.deathday +")";
    aname_div.appendChild(n);
    result_div.appendChild(aname_div);

    let nation_div = document.createElement("div");
    nation_div.classList.add("nation_div");
    let nation = document.createElement("p");
    nation.textContent = artist.nationality
    nation_div.appendChild(nation);
    result_div.appendChild(nation_div);

    let info_div = document.createElement("div");
    info_div.classList.add("info_div");
    let info = document.createElement("p");
    info.textContent = artist.info
    info_div.appendChild(info);
    result_div.appendChild(info_div);

    let bio_div = document.createElement("div");
    bio_div.classList.add("bio_div");
    let bio = document.createElement("p");
    bio.textContent = artist.biography;
    bio_div.appendChild(bio);
    result_div.appendChild(bio_div);

    let loading = document.getElementById("loading1");
    while (loading.firstChild) {
        loading.firstChild.remove();
    }
}

window.onload = load_search;
var slide_index = 0;
