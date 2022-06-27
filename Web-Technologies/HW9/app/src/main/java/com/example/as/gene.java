package com.example.as;

public class gene {
    String gene_img;
    String gene_name;
    String gene_year;
    String gene_info;

    public gene(String gene_img, String gene_name, String gene_year, String gene_info) {
        this.gene_img = gene_img;
        this.gene_name = gene_name;
        this.gene_year = gene_year;
        this.gene_info = gene_info;
    }

    public String getGene_img() {
        return gene_img;
    }

    public void setGene_img(String gene_img) {
        this.gene_img = gene_img;
    }

    public String getGene_name() {
        return gene_name;
    }

    public void setGene_name(String gene_name) {
        this.gene_name = gene_name;
    }

    public String getGene_year() {
        return gene_year;
    }

    public void setGene_year(String gene_year) {
        this.gene_year = gene_year;
    }

    public String getGene_info() {
        return gene_info;
    }

    public void setGene_info(String gene_info) {
        this.gene_info = gene_info;
    }
}
