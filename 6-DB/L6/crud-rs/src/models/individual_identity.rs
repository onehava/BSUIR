use crate::schema::*;
use diesel::{AsChangeset, Identifiable, Insertable, Queryable};
use rocket::serde::{Deserialize, Serialize};

#[derive(Queryable, Identifiable, Serialize)]
#[diesel(table_name = individual_identity)]
pub struct IndividualIdentity {
    pub id: i32,
    pub name: String,
    pub passport_id: String,
}

#[derive(Insertable, Deserialize)]
#[diesel(table_name = individual_identity)]
pub struct NewIndividualIdentity {
    pub name: String,
    pub passport_id: String,
}

#[derive(AsChangeset, Deserialize)]
#[diesel(table_name = individual_identity)]
pub struct UpdateIndividualIdentity {
    pub name: Option<String>,
    pub passport_id: Option<String>,
}