#include "Framebuffer.hpp"

Framebuffer::Framebuffer()
{
    m_framebufferIds.resize(1);
    glGenFramebuffers(1, m_framebufferIds.data());
    m_attachments.insert({0, {}});
}

Framebuffer::Framebuffer(int framebufferCount)
{
    m_framebufferIds.resize(framebufferCount);
    glGenFramebuffers(framebufferCount, m_framebufferIds.data());
    for (int index = 0; index < framebufferCount; index++)
    {
        m_attachments.insert({index, {}});
    }
}

Framebuffer::~Framebuffer()
{
    if (!m_framebufferIds.empty())
    {
        // Delete attached textures first
        m_attachments.clear();

        glDeleteFramebuffers(static_cast<int>(m_framebufferIds.size()), m_framebufferIds.data());
        m_framebufferIds.clear();
    }
}

auto Framebuffer::Count() const -> int
{
    return static_cast<int>(m_framebufferIds.size());
}

void Framebuffer::Bind(int framebufferIndex)
{
    if (framebufferIndex < 0 || framebufferIndex >= static_cast<int>(m_framebufferIds.size()))
    {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferIds.at(framebufferIndex));
}

void Framebuffer::BindRead(int framebufferIndex)
{
    if (framebufferIndex < 0 || framebufferIndex >= static_cast<int>(m_framebufferIds.size()))
    {
        return;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebufferIds.at(framebufferIndex));
}

void Framebuffer::BindDraw(int framebufferIndex)
{
    if (framebufferIndex < 0 || framebufferIndex >= static_cast<int>(m_framebufferIds.size()))
    {
        return;
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebufferIds.at(framebufferIndex));
}

void Framebuffer::SetSize(int width, int height)
{
    if (width == 0 || height == 0 ||
        (width == m_width && height == m_height))
    {
        return;
    }

    m_width = width;
    m_height = height;

    for (auto& attachments : m_attachments)
    {
        Bind(attachments.first);
        for (auto& texture : attachments.second)
        {
            texture.second.SetSize(width, height);
            glFramebufferTexture2D(GL_FRAMEBUFFER, texture.first, GL_TEXTURE_2D, texture.second.TextureId(), 0);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::CreateColorAttachment(int framebufferIndex, int attachmentIndex)
{
    if (framebufferIndex < 0 || framebufferIndex >= static_cast<int>(m_framebufferIds.size()))
    {
        return;
    }

    TextureAttachment textureAttachment{TextureAttachment::AttachmentType::Color, m_width, m_height};
    auto textureId = textureAttachment.TextureId();
    m_attachments.at(framebufferIndex).insert({GL_COLOR_ATTACHMENT0 + attachmentIndex, std::move(textureAttachment)});

    if (m_width > 0 && m_height > 0)
    {
        Bind(framebufferIndex);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, GL_TEXTURE_2D, textureId, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void Framebuffer::CreateDepthAttachment(int framebufferIndex)
{
    if (framebufferIndex < 0 || framebufferIndex >= static_cast<int>(m_framebufferIds.size()))
    {
        return;
    }

    TextureAttachment textureAttachment{TextureAttachment::AttachmentType::Depth, m_width, m_height};
    auto textureId = textureAttachment.TextureId();
    m_attachments.at(framebufferIndex).insert({GL_DEPTH_ATTACHMENT, std::move(textureAttachment)});

    if (m_width > 0 && m_height > 0)
    {
        Bind(framebufferIndex);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void Framebuffer::CreateStencilAttachment(int framebufferIndex)
{
    if (framebufferIndex < 0 || framebufferIndex >= static_cast<int>(m_framebufferIds.size()))
    {
        return;
    }

    TextureAttachment textureAttachment{TextureAttachment::AttachmentType::Stencil, m_width, m_height};
    auto textureId = textureAttachment.TextureId();
    m_attachments.at(framebufferIndex).insert({GL_STENCIL_ATTACHMENT, std::move(textureAttachment)});

    if (m_width > 0 && m_height > 0)
    {
        Bind(framebufferIndex);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void Framebuffer::CreateDepthStencilAttachment(int framebufferIndex)
{
    if (framebufferIndex < 0 || framebufferIndex >= static_cast<int>(m_framebufferIds.size()))
    {
        return;
    }

    TextureAttachment textureAttachment{TextureAttachment::AttachmentType::DepthStencil, m_width, m_height};
    auto textureId = textureAttachment.TextureId();
    m_attachments.at(framebufferIndex).insert({GL_DEPTH_STENCIL_ATTACHMENT, std::move(textureAttachment)});

    if (m_width > 0 && m_height > 0)
    {
        Bind(framebufferIndex);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}