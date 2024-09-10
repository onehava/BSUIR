use crate::schema::*;
use diesel::{AsChangeset, Identifiable, Insertable, Queryable};
use rocket::serde::{Deserialize, Serialize};

#[derive(Queryable, Identifiable, Serialize)]
#[diesel(table_name = apartment_type)]
pub struct ApartmentType {
    pub id: i32,
    pub name: String,
    pub description: Option<String>,
    pub capacity: i32,
    pub cost: i32,
}

#[derive(Insertable, Deserialize)]
#[diesel(table_name = apartment_type)]
pub struct NewApartmentType {
    pub name: String,
    pub description: Option<String>,
    pub capacity: i32,
    pub cost: i32,
}

#[derive(AsChangeset, Deserialize)]
#[diesel(table_name = apartment_type)]
pub struct UpdateApartmentType {
    pub name: Option<String>,
    pub description: Option<String>,
    pub capacity: Option<i32>,
    pub cost: Option<i32>,
}