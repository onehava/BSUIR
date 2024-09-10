use crate::schema::*;
use diesel::{AsChangeset, Identifiable, Insertable, Queryable};
use rocket::serde::{Deserialize, Serialize};

#[derive(Queryable, Identifiable, Serialize)]
#[diesel(table_name = service)]
pub struct Service {
    pub id: i32,
    pub name: String,
    pub description: Option<String>,
    pub cost: i32,
}

#[derive(Insertable, Deserialize)]
#[diesel(table_name = service)]
pub struct NewService {
    pub name: String,
    pub description: Option<String>,
    pub cost: i32,
}

#[derive(AsChangeset, Deserialize)]
#[diesel(table_name = service)]
pub struct UpdateService {
    pub name: Option<String>,
    pub description: Option<String>,
    pub cost: Option<i32>,
}
