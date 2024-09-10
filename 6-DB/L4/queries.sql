\c hotel;

-- 1. Получить список всех двухместных номеров
select all
    name as "Apartments",
    cost as "Cost"
from
    apartment_type
where
    capacity = 2;

-- 2. Найти все открытые акционерные общества среди компаний-заказчиков
select all
    name as "Company",
    taxpayer_id as "Taxpayer ID"
from
    company_identity
where
    name like 'ООО %'
order by
    company_identity;

-- 3. Находим даты проживания всех постояльцев
select
    individual.name as "Guest",
    reservation.check_in as "Check in date",
    reservation.check_out as "Check out date"
from
    individual_identity individual
inner join
    guest on individual.id = guest.individual_id
inner join
    apartment_reservation reservation on guest.reservation_id = reservation.id
order by
    individual.name;

-- 4. Сравнивает все возможные виды апартаментов
select
    A.name as "First",
    B.name as "Second",
    A.cost || ' - ' || B.cost || ' = ' || A.cost - B.cost as "dCost"
from
    apartment_type A
cross join
    apartment_type B
where
    A.id != B.id and A.cost - B.cost >= 0;

-- 5. Получаем информацию о свободных номерах
select
    apartment.no as "No",
    apartment_type.name as "Type"
from
    apartment
left join
    apartment_type on apartment.type_id = apartment_type.id
where
    apartment.free = true;

-- 6. Получаем информацию о бронировании номеров
select
    apartment.no as "No",
    apartment_type.name as "Type",
    case
        when customer.individual_id is not null then individual.name
        when customer.company_id is not null then company.name
    end as "Customer",
    reservation.date as "Reservation date"
from
    customer
full outer join
        apartment_reservation reservation on customer.id = reservation.customer_id
full outer join
        apartment on reservation.apartment_no = apartment.no
left join
        apartment_type on apartment.type_id = apartment_type.id
left join
        individual_identity individual on customer.individual_id = individual.id
left join
        company_identity company on customer.company_id = company.id
order by
    reservation.date desc;

-- 7. Получаем список апартаментов с заявленным кондиционером
select
    apartment.no as "No",
    apartment_type.name as "Type"
from
    apartment
inner join
    apartment_type on apartment.type_id = apartment_type.id
where
    apartment_type.description like '%' || lower('кондиционер') || '%';

-- 8. Получаем информацию об услугах, которые никогда не оказывались
select
    service.name as "Service",
    service.cost as "Cost"
from
    service
left join
    service_order on service.id = service_order.service_id
where
    service_order.service_id is null
order by
    service.cost desc;

-- 9. Получаем информацию о номерах, которые когда-либо бронировались
select distinct
    apartment.no as "No",
    apartment_type.name as "Type"
from
    apartment_reservation reservation
        left join
    apartment on reservation.apartment_no = apartment.no
        left join
    apartment_type on apartment.type_id = apartment_type.id;

-- 10. Получаем информацию об услугах с оформленным оказанием
select distinct
    service.name as "Service",
    service.cost as "Cost"
from
    service
right join
    service_order s_order on service.id = s_order.service_id;

-- 11. Находим все номера с «оригинальными» названиями
select
    apartment_type.name as "Type"
from
    apartment_type
where
    apartment_type.name like '%«%»%';

-- 12. Находим все пересекающиеся бронирования
select
    A.check_in || ' - ' || A.check_out as "First",
    B.check_in || ' - ' || B.check_out as "Second",
    B.check_in || ' - ' || least(A.check_out, B.check_out) as "Intersection"
from
    apartment_reservation A
cross join
    apartment_reservation B
where
    A.id != B.id and A.check_in <= B.check_in and B.check_in <= A.check_out;

-- 13. Находим всех постояльцев, которые проживают в апартаментах, заказанных компанией
select
    individual.name as "Guest",
    apartment.no as "No",
    company.name as "Customer"
from
    guest
inner join
    apartment_reservation reservation on guest.reservation_id = reservation.id
inner join
    customer on reservation.customer_id = customer.id
inner join
    company_identity company on customer.company_id = company.id
inner join
    individual_identity individual on guest.individual_id = individual.id
inner join
    apartment on reservation.apartment_no = apartment.no;

-- 14. Находим все длительные бронирования (не менее одной недели)
select
    reservation.apartment_no as "No",
    reservation.check_in || ' - ' || reservation.check_out as "Period"
from
    apartment_reservation reservation
where
    reservation.check_out - reservation.check_in >= 7;

-- 15. Получаем список услуг, которые должны быть оказаны в день заказа
select distinct
    service.name as "Service",
    service_order.service_date as "Date"
from
    service_order
inner join
    service on service_order.service_id = service.id
where
    service_order.order_date = service_order.service_date
order by
    service_order.service_date, service.name;