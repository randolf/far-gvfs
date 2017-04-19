#include <iostream>
#include "GvfsService.h"

GvfsService::GvfsService() :
    m_mountCount(0)
{
}

bool GvfsService::mount(const std::string &resPath, const std::string &userName,
                        const std::string &password) throw(GvfsServiceException)
{
    m_exception.reset();

    Gio::init();
    // not need because of use Glib::MainLoop
    // Glib::init(); 

    main_loop = Glib::MainLoop::create(false);

    file = Gio::File::create_for_parse_name(resPath);
    Glib::RefPtr<Gio::MountOperation> mount_operation = Gio::MountOperation::create();

    bool l_anonymous = userName.empty() && password.empty();

    if (!userName.empty()) mount_operation->set_username(userName);
    if (!password.empty()) mount_operation->set_password(password);

    // connect mount_operation slots
    mount_operation->signal_ask_password().connect(
    [mount_operation, l_anonymous](const Glib::ustring& msg,
                                   const Glib::ustring& defaultUser,
                                   const Glib::ustring& defaultdomain,
                                   Gio::AskPasswordFlags flags)
    {
std::cerr << "Gvfs on signal_ask_password ask password: " << msg << std::endl
          << "Gvfs on signal_ask_password default user: " << defaultUser << std::endl
          << "Gvfs on signal_ask_password default domain: " << defaultdomain << std::endl;

        if ((flags & G_ASK_PASSWORD_ANONYMOUS_SUPPORTED) && l_anonymous)
        {
std::cerr << "Gvfs on signal_ask_password set anonymous" << std::endl;
            mount_operation->set_anonymous(true);
        }
        else
        {
            // trigger functor for entering user credentials
            if (flags & G_ASK_PASSWORD_NEED_USERNAME)
            {
std::cerr << "Gvfs on signal_ask_password NEED USERNAME" << std::endl;
                // trigger user name enter callback, call passwd functor
            }
            if (flags & G_ASK_PASSWORD_NEED_DOMAIN)
            {
std::cerr << "Gvfs on signal_ask_password NEED DOMAIN" << std::endl;
                // trigger domain name enter callback, call passwd functor
            }
            if (flags & G_ASK_PASSWORD_NEED_PASSWORD)
            {
std::cerr << "Gvfs on signal_ask_password NEED PASSWORD" << std::endl;
                // trigger password name enter callback, call passwd functor
            }
        }
        mount_operation->reply(Gio::MOUNT_OPERATION_HANDLED);
    });
    mount_operation->signal_ask_question().connect(
    [mount_operation](const Glib::ustring& msg, const Glib::StringArrayHandle& choices)
    {
std::cerr << "on signal_ask_question: " << msg << std::endl;
    });

    // do mount
    bool l_mounted = false;
    try
    {
        file->mount_enclosing_volume(mount_operation,
                                     [this] (Glib::RefPtr<Gio::AsyncResult>& result)
                                     {
                                        this->mount_cb(result);
                                     });
        m_mountCount++;
std::cerr << "GvfsService::mount() inc m_mountCount: " << m_mountCount << std::endl;
        if(m_mountCount > 0)
        {
            main_loop->run();
        }
        m_mountName = file->find_enclosing_mount()->get_name();
        m_mountPath = file->find_enclosing_mount()->get_default_location()->get_path();
        std::cout << "GvfsService::mount() name: " << m_mountName << std::endl;
        std::cout << "GvfsService::mount() path: " << m_mountPath << std::endl;
        l_mounted = true;
    }
    catch(const Glib::Error& ex)
    {
        std::cerr << "GvfsService::mount() Glib::Error: " << ex.what() << std::endl
                  << "m_mountCount: " << m_mountCount << std::endl;
        throw *m_exception;
    }
    return l_mounted;
}

bool GvfsService::umount(const std::string &resPath) throw(GvfsServiceException)
{
    m_exception.reset();

    Gio::init();
    // not need because of use Glib::MainLoop
    // Glib::init();

    main_loop = Glib::MainLoop::create(false);

    file = Gio::File::create_for_parse_name(resPath);
    Glib::RefPtr<Gio::MountOperation> mount_operation = Gio::MountOperation::create();

    bool l_unmounted = false;
    try
    {
        Glib::RefPtr<Gio::Mount> mount = file->find_enclosing_mount();
        mount->unmount(mount_operation,
                                     [&l_unmounted, this] (Glib::RefPtr<Gio::AsyncResult>& result)
                                     {
                                         l_unmounted = this->unmount_cb(result);
                                     });
        m_mountCount++;
std::cerr << "GvfsService::umount() inc m_mountCount: " << m_mountCount << std::endl;
        if(m_mountCount > 0)
        {
            main_loop->run();
        }
        
    }
    catch(const Glib::Error& ex)
    {
        std::cerr << "GvfsService::umount() Glib::Error: "<< ex.what() << std::endl
                  << "m_mountCount: " << m_mountCount;
        throw *m_exception;
    }
    return l_unmounted;
}

void GvfsService::mount_cb(Glib::RefPtr<Gio::AsyncResult>& result)
{
std::cerr << "GvfsService::mount_cb()" << std::endl;
    try
    {
        file->mount_enclosing_volume_finish(result);
    }
    catch(const Glib::Error& ex)
    {
        std::cerr << "GvfsService::mount_cb() Glib::Error: "<< ex.what() << std::endl;
        // fill exception
        m_exception = std::make_shared<GvfsServiceException>(ex.domain(), ex.code(), ex.what());
    }

    m_mountCount--;
std::cerr << "GvfsService::mount_cb() dec m_mountCount: " << m_mountCount << std::endl;

    if(m_mountCount == 0)
    {
        main_loop->quit();
    }
}

bool GvfsService::unmount_cb(Glib::RefPtr<Gio::AsyncResult> &result)
{
    Glib::RefPtr<Gio::Mount> mount = Glib::RefPtr<Gio::Mount>::cast_dynamic(result->get_source_object_base());

std::cerr << "GvfsService::unmount_cb() " << mount.operator->() << std::endl;
    bool success = false;
    try
    {
        success = mount->unmount_finish(result);
    }
    catch(const Glib::Error& ex)
    {
        std::cerr << "GvfsService::unmount_cb() Glib::Error: "<< ex.what() << std::endl;
        // fill exception
        m_exception = std::make_shared<GvfsServiceException>(ex.domain(), ex.code(), ex.what());
    }

    m_mountCount--;
std::cerr << "GvfsService::unmount_cb() dec m_mountCount: " << m_mountCount << std::endl;

    if(m_mountCount == 0)
    {
        main_loop->quit();
    }
    return success;
}
