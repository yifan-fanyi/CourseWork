package com.example.as;

public class Artist {
    private String artistname;
    private String artistid;
    private String artistnation;
    private String artistbirth;

    public Artist(String artistname, String artistid, String artistnation, String artistbirth){
        this.artistname = artistname;
        this.artistid = artistid;
        this.artistnation = artistnation;
        this.artistbirth = artistbirth;
    }

    public String getArtistname() {
        return artistname;
    }
    public String getArtistid(){
        return artistid;
    }
    public String getArtistnation(){
        return artistnation;
    }
    public String getArtistbirth(){
        return artistbirth;
    }

}
