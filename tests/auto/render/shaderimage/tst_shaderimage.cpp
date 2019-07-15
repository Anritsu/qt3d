/****************************************************************************
**
** Copyright (C) 2019 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QTest>
#include <Qt3DRender/qshaderimage.h>
#include <Qt3DRender/private/qshaderimage_p.h>
#include <Qt3DRender/private/shaderimage_p.h>
#include <Qt3DCore/qpropertyupdatedchange.h>
#include "qbackendnodetester.h"
#include "testrenderer.h"

class tst_ShaderImage : public Qt3DCore::QBackendNodeTester
{
    Q_OBJECT

private Q_SLOTS:

    void checkInitialState()
    {
        // GIVEN
        Qt3DRender::Render::ShaderImage backendShaderImage;

        // THEN
        QCOMPARE(backendShaderImage.isEnabled(), false);
        QVERIFY(backendShaderImage.peerId().isNull());
        QCOMPARE(backendShaderImage.textureId(), Qt3DCore::QNodeId());
        QCOMPARE(backendShaderImage.mipLevel(), int(0));
        QCOMPARE(backendShaderImage.layer(), int(0));
        QCOMPARE(backendShaderImage.layered(), false);
        QCOMPARE(backendShaderImage.access(), Qt3DRender::QShaderImage::ReadWrite);
        QCOMPARE(backendShaderImage.format(), Qt3DRender::QShaderImage::NoFormat);
    }

    void checkCleanupState()
    {
        // GIVEN
        Qt3DRender::Render::ShaderImage backendShaderImage;

        // WHEN
        backendShaderImage.setEnabled(true);
        backendShaderImage.setTextureId(Qt3DCore::QNodeId::createId());
        backendShaderImage.setMipLevel(8);
        backendShaderImage.setLayer(200);
        backendShaderImage.setLayered(true);
        backendShaderImage.setAccess(Qt3DRender::QShaderImage::WriteOnly);
        backendShaderImage.setFormat(Qt3DRender::QShaderImage::R32F);

        backendShaderImage.cleanup();

        // THEN
        QCOMPARE(backendShaderImage.isEnabled(), false);
        QCOMPARE(backendShaderImage.textureId(), Qt3DCore::QNodeId());
        QCOMPARE(backendShaderImage.mipLevel(), int(0));
        QCOMPARE(backendShaderImage.layer(), int(0));
        QCOMPARE(backendShaderImage.layered(), false);
        QCOMPARE(backendShaderImage.access(), Qt3DRender::QShaderImage::ReadWrite);
        QCOMPARE(backendShaderImage.format(), Qt3DRender::QShaderImage::NoFormat);

    }

    void checkInitializeFromPeer()
    {
        // GIVEN
        Qt3DRender::QShaderImage shaderImage;

        {
            // WHEN
            Qt3DRender::Render::ShaderImage backendShaderImage;
            simulateInitialization(&shaderImage, &backendShaderImage);

            // THEN
            QCOMPARE(backendShaderImage.isEnabled(), true);
            QCOMPARE(backendShaderImage.peerId(), shaderImage.id());
            QCOMPARE(backendShaderImage.textureId(), Qt3DCore::QNodeId());
            QCOMPARE(backendShaderImage.mipLevel(), 0);
            QCOMPARE(backendShaderImage.layer(), 0);
            QCOMPARE(backendShaderImage.layered(), false);
            QCOMPARE(backendShaderImage.access(),  Qt3DRender::QShaderImage::ReadWrite);
            QCOMPARE(backendShaderImage.format(),  Qt3DRender::QShaderImage::Automatic);
        }
        {
            // WHEN
            Qt3DRender::Render::ShaderImage backendShaderImage;
            shaderImage.setEnabled(false);
            simulateInitialization(&shaderImage, &backendShaderImage);

            // THEN
            QCOMPARE(backendShaderImage.peerId(), shaderImage.id());
            QCOMPARE(backendShaderImage.isEnabled(), false);
        }
    }

    void checkSceneChangeEvents()
    {
        // GIVEN
        Qt3DRender::Render::ShaderImage backendShaderImage;
        TestRenderer renderer;
        backendShaderImage.setRenderer(&renderer);

        {
             // WHEN
             const bool newValue = false;
             const auto change = Qt3DCore::QPropertyUpdatedChangePtr::create(Qt3DCore::QNodeId());
             change->setPropertyName("enabled");
             change->setValue(newValue);
             backendShaderImage.sceneChangeEvent(change);

             // THEN
            QCOMPARE(backendShaderImage.isEnabled(), newValue);
        }
        {
             // WHEN
             const Qt3DCore::QNodeId newValue = Qt3DCore::QNodeId::createId();
             const auto change = Qt3DCore::QPropertyUpdatedChangePtr::create(Qt3DCore::QNodeId());
             change->setPropertyName("texture");
             change->setValue(QVariant::fromValue(newValue));
             backendShaderImage.sceneChangeEvent(change);

             // THEN
            QCOMPARE(backendShaderImage.textureId(), newValue);
        }
        {
             // WHEN
             const int newValue = 883;
             const auto change = Qt3DCore::QPropertyUpdatedChangePtr::create(Qt3DCore::QNodeId());
             change->setPropertyName("mipLevel");
             change->setValue(QVariant::fromValue(newValue));
             backendShaderImage.sceneChangeEvent(change);

             // THEN
            QCOMPARE(backendShaderImage.mipLevel(), newValue);
        }
        {
             // WHEN
             const int newValue = 1584;
             const auto change = Qt3DCore::QPropertyUpdatedChangePtr::create(Qt3DCore::QNodeId());
             change->setPropertyName("layer");
             change->setValue(QVariant::fromValue(newValue));
             backendShaderImage.sceneChangeEvent(change);

             // THEN
            QCOMPARE(backendShaderImage.layer(), newValue);
        }
        {
             // WHEN
             const bool newValue = true;
             const auto change = Qt3DCore::QPropertyUpdatedChangePtr::create(Qt3DCore::QNodeId());
             change->setPropertyName("layered");
             change->setValue(QVariant::fromValue(newValue));
             backendShaderImage.sceneChangeEvent(change);

             // THEN
            QCOMPARE(backendShaderImage.layered(), newValue);
        }
        {
             // WHEN
             const  Qt3DRender::QShaderImage::Access newValue = Qt3DRender::QShaderImage::WriteOnly;
             const auto change = Qt3DCore::QPropertyUpdatedChangePtr::create(Qt3DCore::QNodeId());
             change->setPropertyName("access");
             change->setValue(QVariant::fromValue(newValue));
             backendShaderImage.sceneChangeEvent(change);

             // THEN
            QCOMPARE(backendShaderImage.access(), newValue);
        }
        {
             // WHEN
             const  Qt3DRender::QShaderImage::ImageFormat newValue =  Qt3DRender::QShaderImage::RG16F;
             const auto change = Qt3DCore::QPropertyUpdatedChangePtr::create(Qt3DCore::QNodeId());
             change->setPropertyName("format");
             change->setValue(QVariant::fromValue(newValue));
             backendShaderImage.sceneChangeEvent(change);

             // THEN
            QCOMPARE(backendShaderImage.format(), newValue);
        }
    }

};

QTEST_MAIN(tst_ShaderImage)

#include "tst_shaderimage.moc"
