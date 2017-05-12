# far-gvfs (gvfspanel)

Альфа-версия, **не для повседневного использования!**

_Только для особо интересующихся._

Очень простое дополнение к [far2l](https://github.com/elfmz/far2l). Обертка
вокруг GVFS: позволяет работать с сетевыми ресурсами как с виртуальными
файловыми системами. Поддерживаются те протоколы, которые поддерживает GVFS
(SFTP, WebDAV, SAMBA и др.).

#### Лицензия: MIT

Использован код из https://github.com/invy/far2l/tree/gvfs/gvfspanel.

## Зависимости

* gtkmm-3.0 (возможно, будет работать и с более ранней 2 версией);
* libuuid;
* OpenSSL версии 1.0.2 и старше.

Установки зависимостей сборки в Debian (Ubuntu):

```
apt-get install libgtkmm-3.0-dev uuid-dev libssl-dev
```

OpenSSL необходима для шифрования хранимых паролей. _Обратите внимание:_ если
дополнение собирается без OpenSSL, то пароли хранятся практически
незашифрованными!

Для сборки дополнение помещается в дерево исходного кода far2l в виде
поддиректории, например, если код far2l развернут в директорию "far2l",
то код far-gvfs помещается в "far2l/far-gvfs". Затем директорию добавляют
в проектный файл "CMakeLists.txt" far2l:

```
...
add_subdirectory (align)
add_subdirectory (autowrap)
add_subdirectory (drawline)
add_subdirectory (far-gvfs)
...
```

Дополнение будет собрано и подготовлено к установке вместе с прочими
дополнениями far2l, входящими в его состав.

Для работы дополнения в целевой ОС должны быть установлены:

* gtkmm;
* поддержка GVFS;
* libuuid (пакет libuuid1 в Debian);
* libssl (необязательно).

## Возможности

Для монтирования сетевых ресурсов необходимо указать URL ресурса, учетную
запись и пароль для нужд аутентификации. Анонимное монтирование возможно:
попытки анонимного подключения выполняются, если и учетная запись, и пароль
_не указаны_. Вместо хранения пароля можно запрашивать его перед каждым
монтированием ресурса. В случае возникновения каких-либо ошибок подключения
выдается сообщение о них в том виде, в котором его предоставляет GVFS. В
случае удачного завершения операции панель GVFS переключается на вновь
подмонтированный ресурс. Ресурсы в статусе подмонтированных в перечне отмечены
знаком "*". Отключить ресурс можно, нажав Shift-F8. Статусы ресурсов на панели
обновляются по нажатию Ctrl-R.

В зависиомости от настроек дополнения, известные смонтированные ресурсы при
завершении работы с far2l можно автоматически отсоединять. Если запущено более
одного экземпляра far2l, то они пользуются общим (системным) пулом
смонтированных ресурсов. Таким образом, разрешение всех возможных конфликтов в
работе с ресурсами оставлено средствам самой GVFS.

Данные о сетевых ресурсах хранятся в реестре far2l в ветке
"Software/Far2/gvfspanel/Resources". Если ранее сохраненные данные перестали
отображаться на панели или неправильно загружаются, удалите ветку целиком и
заведите ресурсы заново.

Дополнение добавляется в меню выбора устройств far2l под именем "GVFS", а
также в меню конфигурации и команд дополнений. Параметры конфигурации far-gvfs:

* отсоединять или нет известные смонтированные ресурсы при выходе из far2l. По
  умолчанию отсоединяются.

Команды:

* переключение текущей панели на панель самого дополнения.

## Известные проблемы

* Возникающая спорадически ошибка монтирования 20005. В основном безвредна,
  потому что ресурс фактически монтируется успешно. Подробности в
  [#14](https://github.com/cycleg/far-gvfs/issues/14).
