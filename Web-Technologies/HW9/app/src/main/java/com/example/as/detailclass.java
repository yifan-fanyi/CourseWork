package com.example.as;

public class detailclass {
    String name;
    String nation;
    String id;
    String birth;
    String death;
    String bio;

    public detailclass(String name, String nation, String id, String birth, String death, String bio) {
        this.name = name;
        this.nation = nation;
        this.id = id;
        this.birth = birth;
        this.death = death;
        this.bio = bio;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getNation() {
        return nation;
    }

    public void setNation(String nation) {
        this.nation = nation;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getBirth() {
        return birth;
    }

    public void setBirth(String birth) {
        this.birth = birth;
    }

    public String getDeath() {
        return death;
    }

    public void setDeath(String death) {
        this.death = death;
    }

    public String getBio() {
        return bio;
    }

    public void setBio(String bio) {
        this.bio = bio;
    }
}
