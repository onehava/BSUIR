use crate::schema::*;
use diesel::{AsChangeset, Identifiable, Insertable, Queryable};
use rocket::serde::{Deserialize, Serialize};

#[derive(Queryable, Identifiable, Serialize)]
#[diesel(table_name = guest)]
pub struct Guest {
    pub id: i32,
    pub individual_id: i32,
    pub reservation_id: i32,
}

#[derive(Insertable, Deserialize)]
#[diesel(table_name = guest)]
pub struct NewGuest {
    pub individual_id: i32,
    pub reservation_id: i32,
}

#[derive(AsChangeset, Deserialize)]
#[diesel(table_name = guest)]
pub struct UpdateGuest {
    pub individual_id: Option<i32>,
    pub reservation_id: Option<i32>,
}
