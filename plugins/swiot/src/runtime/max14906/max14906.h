#ifndef MAX14906TOOL_H
#define MAX14906TOOL_H

#include "pluginbase/toolmenuentry.h"
#include "ui_max14906.h"
#include "diocontroller.h"
#include "diodigitalchannelcontroller.h"
#include "src/runtime/readerthread.h"
#include "diosettingstab.h"
#include <gui/tool_view.hpp>
#include <gui/flexgridlayout.hpp>

namespace scopy::swiot {
#define MAX_NAME "max14906"
class DioDigitalChannel;
class DioController;

class Max14906 : public QWidget {
	Q_OBJECT
public:
	explicit Max14906(struct iio_context *ctx, ToolMenuEntry *tme, QWidget *parent = nullptr);
	~Max14906() override;

Q_SIGNALS:
        void backBtnPressed();

private Q_SLOTS:
	void runButtonToggled();
	void singleButtonToggled();

	void timerChanged(double value);

private:
        static QPushButton* createBackButton();
	void initChannels();
	void setupDynamicUi(QWidget *parent);
	void initMonitorToolView();
	scopy::gui::GenericMenu* createGeneralSettings(const QString& title, QColor* color);
	void connectSignalsAndSlots();

	static QFrame* createVLine(QWidget* parent);
	static QFrame* createHLine(QWidget* parent);
	static QMainWindow* createDockableMainWindow(const QString& title, DioDigitalChannel* digitalChannel, QWidget* parent);

        QPushButton *m_backButton;
	DioController *max14906ToolController;
	Ui::Max14906 *ui;
	DioSettingsTab *m_max14906SettingsTab;
	scopy::gui::ToolView *m_toolView;
	scopy::gui::GenericMenu* m_generalSettingsMenu;
	scopy::gui::SubsectionSeparator *settingsWidgetSeparator;
	QTimer *m_qTimer;

	ReaderThread *m_readerThread;
	QMap<int, DioDigitalChannelController*> m_channelControls;

	ToolMenuEntry *m_tme;
};
}
#endif // MAX14906TOOL_H
