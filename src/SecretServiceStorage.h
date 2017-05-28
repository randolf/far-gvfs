///
/// @file SecretServiceStorage.h
/// @author cycleg
///
/*
 * Created on: 26.05.2017
 */
#pragma once

#include <string>
#include <libsecret/secret.h>
#include <gtkmm.h>

///
/// @brief Хранилище паролей в системных безопасных хранилищах.

/// Класс представляет собой обертку над API libsecret. Поддерживаются три
/// операции:
///
/// * сохранить пароль;
/// * извлечь пароль;
/// * удалить пароль.
///
/// В зависимости от используемого окружения рабочего стола пароли хранятся в
/// Gnome Keyring или KDE Wallet.
///
/// Пароли помещаются в коллекцию по умолчанию (SECRET_COLLECTION_DEFAULT) по
/// внутренней схеме (SECRET_SERVICE_STORAGE_SCHEMA). Ключом для поиска пароля
/// в коллекции выступают имя пользователя для аутентификации на
/// ресурсе, сервер и протокол из URL ресурса.
///
/// Хотя внутри операции выполняются асинхронно, интерфейс самого класса --
/// синхронный. После завершения одной из трех вышеуказанных операций в этом
/// же экземпляре может быть запущена другая. Однако класс -- не
/// реентерабельный!
///
/// @author cycleg
///
class SecretServiceStorage
{
  public:
    ///
    /// Конструктор.
    ///
    SecretServiceStorage();

    ///
    /// Сохранить пароль для данного URL в связке.
    ///
    /// @param [in] url URL ресурса.
    /// @param [in] user Имя пользователя.
    /// @param [in] password Сохраняемый пароль.
    /// @return Результат операции.
    ///
    /// Возвращает false, если сохранить не удалось.
    ///
    bool SavePassword(const std::wstring& url, const std::wstring& user,
                      const std::wstring& password);
    ///
    /// Найти пароль для данного URL в связке.
    ///
    /// @param [in] url URL ресурса.
    /// @param [in] user Имя пользователя.
    /// @param [out] password Найденный пароль.
    /// @return Результат операции.
    ///
    /// Возвращает false, если пароль не найден, в password -- пустая строка.
    ///
    bool LoadPassword(const std::wstring& url, const std::wstring& user,
                      std::wstring& password);
    ///
    /// Удалить из связки пароль для данного URL.
    ///
    /// @param [in] url URL ресурса.
    /// @param [in] user Имя пользователя.
    ///
    void RemovePassword(const std::wstring& url, const std::wstring& user);

  private:
    ///
    /// Обратный вызов после сохранения пароля.
    ///
    /// @param [in] source Объект, инициировавший асинхронную операцию.
    /// @param [in] result Результат операции.
    /// @param [in] user_data В текущей реализации всегда nullptr.
    ///
    void onPasswordStored(GObject* source, GAsyncResult* result,
                          gpointer user_data);
    ///
    /// Обратный вызов после завершения поиска пароля.
    ///
    /// @param [in] source Объект, инициировавший асинхронную операцию.
    /// @param [in] result Результат операции.
    /// @param [in] user_data В текущей реализации всегда nullptr.
    ///
    void onPasswordFound(GObject* source, GAsyncResult* result,
                         gpointer user_data);
    ///
    /// Обратный вызов после удаления пароля.
    ///
    /// @param [in] source Объект, инициировавший асинхронную операцию.
    /// @param [in] result Результат операции.
    /// @param [in] user_data В текущей реализации всегда nullptr.
    ///
    void onPasswordRemoved(GObject* source, GAsyncResult* result,
                           gpointer user_data);

    bool m_result; ///< Результат последней асинхронной операции.
    std::string m_password; ///< Буфер для найденного пароля. Используется
                            ///< в LoadPassword().
    Glib::RefPtr<Glib::MainLoop> m_mainLoop; ///< Главный цикл glib.
};