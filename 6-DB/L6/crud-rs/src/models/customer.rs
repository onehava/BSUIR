use crate::schema::*;
use diesel::{AsChangeset, Identifiable, Insertable, Queryable};
use rocket::serde::{Deserialize, Serialize};

#[derive(Queryable, Identifiable, Serialize)]
#[diesel(table_name = customer)]
pub struct Customer {
    pub id: i32,
    pub individual_id: Option<i32>,
    pub company_id: Option<i32>,
}

#[derive(Insertable, Deserialize)]
#[diesel(table_name = customer)]
pub struct NewCustomer {
    pub individual_id: Option<i32>,
    pub company_id: Option<i32>,
}

#[derive(AsChangeset, Deserialize)]
#[diesel(table_name = customer)]
pub struct UpdateCustomer {
    pub individual_id: Option<i32>,
    pub company_id: Option<i32>,
}

