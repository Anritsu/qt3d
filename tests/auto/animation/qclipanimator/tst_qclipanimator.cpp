/****************************************************************************
**
** Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtTest/QTest>
#include <Qt3DAnimation/qanimationclip.h>
#include <Qt3DAnimation/qclipanimator.h>
#include <Qt3DAnimation/private/qanimationclip_p.h>
#include <Qt3DAnimation/private/qclipanimator_p.h>
#include <Qt3DCore/qpropertyupdatedchange.h>
#include <Qt3DCore/qnodecreatedchange.h>
#include <Qt3DCore/private/qnodecreatedchangegenerator_p.h>
#include <QObject>
#include <QSignalSpy>
#include <testpostmanarbiter.h>

class tst_QClipAnimator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
        qRegisterMetaType<Qt3DAnimation::QAnimationClip*>();
    }

    void checkDefaultConstruction()
    {
        // GIVEN
        Qt3DAnimation::QClipAnimator animator;

        // THEN
        QCOMPARE(animator.clip(), static_cast<Qt3DAnimation::QAnimationClip *>(nullptr));
    }

    void checkPropertyChanges()
    {
        // GIVEN
        Qt3DAnimation::QClipAnimator animator;

        {
            // WHEN
            QSignalSpy spy(&animator, SIGNAL(clipChanged(Qt3DAnimation::QAnimationClip *)));
            auto newValue = new Qt3DAnimation::QAnimationClip();
            animator.setClip(newValue);

            // THEN
            QVERIFY(spy.isValid());
            QCOMPARE(animator.clip(), newValue);
            QCOMPARE(newValue->parent(), &animator);
            QCOMPARE(spy.count(), 1);

            // WHEN
            spy.clear();
            animator.setClip(newValue);

            // THEN
            QCOMPARE(animator.clip(), newValue);
            QCOMPARE(spy.count(), 0);
        }
    }

    void checkCreationData()
    {
        // GIVEN
        Qt3DAnimation::QClipAnimator animator;
        auto clip = new Qt3DAnimation::QAnimationClip();
        animator.setClip(clip);

        // WHEN
        QVector<Qt3DCore::QNodeCreatedChangeBasePtr> creationChanges;
        {
            Qt3DCore::QNodeCreatedChangeGenerator creationChangeGenerator(&animator);
            creationChanges = creationChangeGenerator.creationChanges();
        }

        // THEN
        {
            QCOMPARE(creationChanges.size(), 2);

            const auto creationChangeData = qSharedPointerCast<Qt3DCore::QNodeCreatedChange<Qt3DAnimation::QClipAnimatorData>>(creationChanges.first());
            const Qt3DAnimation::QClipAnimatorData data = creationChangeData->data;

            QCOMPARE(animator.id(), creationChangeData->subjectId());
            QCOMPARE(animator.isEnabled(), true);
            QCOMPARE(animator.isEnabled(), creationChangeData->isNodeEnabled());
            QCOMPARE(animator.metaObject(), creationChangeData->metaObject());
            QCOMPARE(animator.clip()->id(), data.clipId);
        }

        // WHEN
        animator.setEnabled(false);
        {
            Qt3DCore::QNodeCreatedChangeGenerator creationChangeGenerator(&animator);
            creationChanges = creationChangeGenerator.creationChanges();
        }

        // THEN
        {
            QCOMPARE(creationChanges.size(), 2);

            const auto creationChangeData = qSharedPointerCast<Qt3DCore::QNodeCreatedChange<Qt3DAnimation::QClipAnimatorData>>(creationChanges.first());

            QCOMPARE(animator.id(), creationChangeData->subjectId());
            QCOMPARE(animator.isEnabled(), false);
            QCOMPARE(animator.isEnabled(), creationChangeData->isNodeEnabled());
            QCOMPARE(animator.metaObject(), creationChangeData->metaObject());
        }
    }

    void checkClipUpdate()
    {
        // GIVEN
        TestArbiter arbiter;
        Qt3DAnimation::QClipAnimator animator;
        auto clip = new Qt3DAnimation::QAnimationClip();
        arbiter.setArbiterOnNode(&animator);

        {
            // WHEN
            animator.setClip(clip);
            QCoreApplication::processEvents();

            // THEN
            QCOMPARE(arbiter.events.size(), 1);
            auto change = arbiter.events.first().staticCast<Qt3DCore::QPropertyUpdatedChange>();
            QCOMPARE(change->propertyName(), "clip");
            QCOMPARE(change->type(), Qt3DCore::PropertyUpdated);
            QCOMPARE(change->value().value<Qt3DCore::QNodeId>(), clip->id());

            arbiter.events.clear();
        }

        {
            // WHEN
            animator.setClip(clip);
            QCoreApplication::processEvents();

            // THEN
            QCOMPARE(arbiter.events.size(), 0);
        }

    }
};

QTEST_MAIN(tst_QClipAnimator)

#include "tst_qclipanimator.moc"
