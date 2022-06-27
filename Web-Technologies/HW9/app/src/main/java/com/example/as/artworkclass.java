package com.example.as;

public class artworkclass {
    String artwork_name;
    String artwork_id;
    String artwork_image;

    public artworkclass(String artwork_name, String artwork_id, String artwork_image) {
        this.artwork_image = artwork_image;
        this.artwork_id = artwork_id;
        this.artwork_name = artwork_name;
    }

    public String getArtwork_image() {
        return artwork_image;
    }

    public void setArtwork_image(String artwork_image) {
        this.artwork_image = artwork_image;
    }

    public String getArtwork_id() {
        return artwork_id;
    }

    public void setArtwork_id(String artwork_id) {
        this.artwork_id = artwork_id;
    }

    public String getArtwork_name() {
        return artwork_name;
    }

    public void setArtwork_name(String artwork_name) {
        this.artwork_name = artwork_name;
    }
}
