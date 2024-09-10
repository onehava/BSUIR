use rocket::{Route, routes};

mod apartment;
mod apartment_reservation;
mod apartment_type;
mod company_identity;
mod customer;
mod guest;
mod individual_identity;
mod service;
mod service_order;

pub fn api_routes() -> Vec<Route> {
    routes![
        apartment::create,
        apartment::list,
        apartment::read,
        apartment::update,
        apartment::delete,

        apartment_reservation::create,
        apartment_reservation::list,
        apartment_reservation::read,
        apartment_reservation::update,
        apartment_reservation::delete,

        apartment_type::create,
        apartment_type::list,
        apartment_type::read,
        apartment_type::update,
        apartment_type::delete,

        company_identity::create,
        company_identity::list,
        company_identity::read,
        company_identity::update,
        company_identity::delete,

        customer::create,
        customer::list,
        customer::read,
        customer::update,
        customer::delete,

        guest::create,
        guest::list,
        guest::read,
        guest::update,
        guest::delete,

        individual_identity::create,
        individual_identity::list,
        individual_identity::read,
        individual_identity::update,
        individual_identity::delete,

        service::create,
        service::list,
        service::read,
        service::update,
        service::delete,

        service_order::create,
        service_order::list,
        service_order::read,
        service_order::update,
        service_order::delete
    ]
}
