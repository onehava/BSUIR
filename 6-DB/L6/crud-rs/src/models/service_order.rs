use crate::schema::*;
use diesel::{AsChangeset, Identifiable, Insertable, Queryable};
use diesel::internal::derives::multiconnection::chrono::NaiveDate;
use rocket::serde::{Deserialize, Serialize};

#[derive(Queryable, Identifiable, Serialize)]
#[diesel(primary_key(guest_id, service_id))]
#[diesel(table_name = service_order)]
pub struct ServiceOrder {
    pub guest_id: i32,
    pub service_id: i32,
    pub order_date: NaiveDate,
    pub service_date: NaiveDate,
    pub paid: bool,
}

#[derive(Insertable, Deserialize)]
#[diesel(primary_key(guest_id, service_id))]
#[diesel(table_name = service_order)]
pub struct NewServiceOrder {
    pub guest_id: i32,
    pub service_id: i32,
    pub order_date: NaiveDate,
    pub service_date: NaiveDate,
    pub paid: bool,
}

#[derive(AsChangeset, Deserialize)]
#[diesel(primary_key(guest_id, service_id))]
#[diesel(table_name = service_order)]
pub struct UpdateServiceOrder {
    pub order_date: Option<NaiveDate>,
    pub service_date: Option<NaiveDate>,
    pub paid: Option<bool>,
}
