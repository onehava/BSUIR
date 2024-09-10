use crate::schema::*;
use diesel::{AsChangeset, Identifiable, Insertable, Queryable};
use rocket::serde::{Deserialize, Serialize};

#[derive(Queryable, Identifiable, Serialize)]
#[diesel(primary_key(no))]
#[diesel(table_name = apartment)]
pub struct Apartment {
    pub no: i32,
    pub free: bool,
    pub type_id: i32,
}

#[derive(Insertable, Deserialize)]
#[diesel(primary_key(no))]
#[diesel(table_name = apartment)]
pub struct NewApartment {
    pub no: i32,
    pub free: bool,
    pub type_id: i32,
}

#[derive(AsChangeset, Deserialize)]
#[diesel(table_name = apartment)]
pub struct UpdateApartment {
    pub free: Option<bool>,
    pub type_id: Option<i32>,
}
