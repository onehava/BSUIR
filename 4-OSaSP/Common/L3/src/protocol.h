#pragma once

enum parent {
    MSG_KILL, // завершить
    MSG_STAT, // запросить статистику
};

enum child {
    MSG_ASK, // запросить разрешение на отправку статистики
    MSG_END, // завершить отправку статистики
};