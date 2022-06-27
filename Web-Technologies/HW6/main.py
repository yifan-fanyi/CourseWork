from flask import Flask, jsonify, request
import requests
import json
app = Flask(__name__)

class HW6():
    def __init__(self, ClientId="239ffa0a7fe2d76b644b", ClientSecret="02e6575dfe6cc023dad6f6fa7b6c9d72"):
        self.ClientId = ClientId
        self.ClientSecret = ClientSecret
        self.token = None

    def get_token(self):
        tmp =  requests.post("https://api.artsy.net/api/tokens/xapp_token", 
                {"client_id": self.ClientId, "client_secret": self.ClientSecret }).content
        tmp = json.loads(tmp)
        self.token = tmp['token']
        return self

    def search(self, artist):
        url = "https://api.artsy.net/api/search?q="+artist.replace(' ', '-').lower()
        r = requests.get(url,
                         headers={'X-XAPP-Token': self.token},
                         params={'similar_to_artist_id': None,
                                'similarity_type': None,
                                'published': None,
                                'artwork_id': None})
        r = r.json()
        result = []
        
        for res in r['_embedded']['results']:
            imgurl = res["_links"]["thumbnail"]["href"]
            if imgurl == "/assets/shared/missing_image.png":
                imgurl = "./static/logo.png"
            if res['og_type'] == 'artist':
                tmp = {'title': res['title'],
                        'image':imgurl,
                        'ID': res["_links"]["self"]["href"].split('/')[-1]}
                result.append(tmp)
        return result

    def detail(self, artist_id):
        url = "https://api.artsy.net/api/artists/" + artist_id
        r = requests.get(url, 
                        headers={'X-XAPP-Token': self.token})
        r = r.json()

        bio = ''
        if len(r['biography']) == 0:
            bio += 'No biography. '
        if len(r['deathday']) == 0:
            bio += "No deathday. "
        if len(r['birthday']) == 0:
            bio += "No birthday. "
        if len(r['nationality']) == 0:
            bio += "No nationality. "
        return {'name':     r['name'], 
                'birthday': r['birthday'], 
                'deathday': r['deathday'], 
                'nationality': r['nationality'],
                'info':bio,
                'biography': r['biography']}

    def unit_test(self):
        self.get_token()
        resa = self.search("picasso")
        resb = self.detail(resa[0]['ID'])
        print(resb)
        
@app.route('/detail/', methods=['GET'])
def detail():
    args = request.args.to_dict()
    hw = HW6().get_token()
    return jsonify(result=hw.detail(args["artist_id"]))

@app.route('/search/', methods=['GET'])
def search():
    args = request.args.to_dict()
    hw = HW6().get_token()
    return jsonify(result=hw.search(args["artist"]))

@app.route('/')
def index():
    return app.send_static_file("csci571_0525.html")

if __name__ == '__main__':
    app.run(debug=True)
    #HW6().unit_test()

    
