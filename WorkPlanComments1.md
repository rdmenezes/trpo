# Введение #

Автором данного документа является Мамонтов Д.П. Я решил несколько пояснить, что я хотел бы увидеть спустя две недели работы.


# Сухареву К.А. #

Тебе нужно разработать объектную модель. Таково первое задание. Объектная модель должна содержать объекты, которые берутся из диаграммы Use-Case, (trpoX.asta раздел загрузки). Соответственно, должна быть таблица

| Объект | Атрибуты | Методы |
|:-------------|:-----------------|:-------------|

Методы - это действия которые производятся над объектом в данной диаграмме.

Атрибуты - это свойства объекта, которые изменяются или инициализируются в процессе выполнения. Всё это берётся из диаграммы и описывается здесь.
Не забудь, что у тебя мало времени и **ЛУЧШЕ** выполнить это задание заранее. И да, это **НЕ** пустые слова, если это не будет выполнено заранее шанс не успеть выполнить следующее задание возрастает сильно.

Следующее задание - это физическая модель. Здесь нужно редактировать саму trpoX.asta . Тебе нужно построить UML-диаграмму, которая описывает все существующие классы программы с их реальными именами и методами. Просьба по возможности прикреплять аннотации, показывающие основные связи. Я не прошу дотошно документировать совершенно каждый метод ( особенно всякие геттеры), но основная суть взаимодействий должна быть понятна. Это чужой пока для тебя код, но в котором надо разобраться.

Третье задание - чисто по кодированию. Сейчас под MacOS у нас две проблемы:

  1. Неправильно обрабатывается указатель (не тот тип)
  1. Странный артефакт при загрузке (непонятный иероглиф).

Первый, я думаю можно решить без проблем (файл saveloadutils.h, функции stringToPtr и ptrToString). Но во второй я не уверен. Есть шанс того, что это вообще не наш баг, а вообще баг Qt под MacOS. Однако, я бы хотел попросить тебя отладить, как минимум посмотреть этот момент со стороны кода.

# Гончаровой И.В. #

Я доволен, что ты находишь баги. Но одна важная просьба - описывай их нормально.

Я понимаю, что ты переняла опыт других людей, которые не описывали баги в процессе тестирования, а просто говорили что они есть. Но, между ними и нами есть разница: их цель - проверить что они есть, наша - найти их и исправить.

Мы на одной стороне, нам обоим сдавать. Более того, ответственность тестера выше, в плане, что если баг просачивается в релиз, то тестеру достаётся в первую очередь.

Чтобы не быть голословным, опишу как должен быть сформирован корректный багрепорт.

Очень плохо, никуда не годится:

ВОТ Я СЮДА КЛИКНУЛА ДОБАВИЛА СЮДА СЮДА СЮДА БЛОКИ ПОТОМ ДОБАВИЛА ЛИНИЮ АННОТАЦИИ НАЖАЛА ПЕРЕДВИНУЛА НАКЛАДЫВАЕТСЯ !!!1111

Плохо:

  1. Добавить блок
  1. Добавить блок
  1. Добавить блок
  1. Добавить блок
  1. Добавить блок
  1. Щелкнуть линию аннотации
  1. Щелкнуть на блок
  1. Начать изменение размера линии аннотации, перекинуть вторую точку через блок
  1. Отпустить клавишу мыши.

Здесь необходимо отметить почему это плохо. Здесь не было указано, в каком направлении можно была направлена линия аннотации сначала. Направление определяется конечной и начальной точками линии (как у вектора). Здесь не было указано, в какую сторону изменялся размер линии. Здесь не было указано, в какую сторону от линии находился блок. И последнее, что плохо: было указано 3-4 совершенно лишних действия. Старайся выделять минимально воспроизводимый случай, этим ты избавишь меня и себя от кучи лишней работы и скриншот нужен не будет.

Идеальный вариант багрепорта - это когда ты ещё прошлась по стеку вызовов и попыталась понять, почему не работает, как минимум выдвинула список возможных гипотез, исходя из архитектуры проекта и информации в процессе отладки. Комбинируй тестирование методом "чёрного ящика" и "прозрачного ящика". Я не требую совершенно идеальных багрепортов, но хотел бы попросить, чтобы ты постаралась максимально к этому приблизиться.

Ещё пара советов: прочитай сценарий работы перед тем, как что-то делать. Всё-таки, у нас возникали проблемы, когда ты пыталась делать то, что не соответствует сценарию (к примеру двигать линии аннотации после исправления бага через несоответствующее поведение). Кроме того, если возник баг НЕ продолжай тестирование в том же процессе программы. Возможно, что есть баги, которые могут изменить поведение программы на некорректное. В принципе ты можешь описать, на какое оно меняется (как постэффект, к примеру программа зависает), но не проводи новые тесты там же, потому что баг может повлиять на них.

Совет по оформлению: используй вкладку Issues и, конкретно, кнопку New Issue для написания багрепортов. Так мы сможем отмониторить процесс исправления бага. В будущем, мне потребуется твоя помощь для проведения регрессионного тестирования.

И, напоследок, кратко опишу, что было найдено сегодня:

  1. Артефакт при соединении двух стрелок (уголки не исчезали, однако при полной перерисовке исчезали, выявилось при неких условиях)
  1. Наложение линии аннотации на блок при изменении размера
  1. Баг, когда мы пытаемся провести стрелку из центра блока в сторону его выхода (не проводится)
  1. Не двигается вертикальная и горизонтальная стрелка, если она является центральным сегментом двух противоположно направленных сегментов
  1. При каких-то непонятных манипуляциях со стрелками программа зависает
  1. При каких-то неизвестных манипуляциях со стрелками щелчок на конце одной из них заставляет программу вылетать с Access Violation (далее AV)

Прошу рассмотреть в ближайшее время и описать подробнее.