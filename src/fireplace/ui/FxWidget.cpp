#include "FxWidget.h"

// Recovered bodies from FireIncludeCpp2.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

void FxWidget::SetID(long id) { m_id = id; }

void FxWidget::AssignWidgetHandler(FWidgetHandler *handler) { m_handler = handler; }

void FxWidget::ParseHelp(char *help) {  }

char * FxWidget::GetText() { return m_text; }

void FxWidget::ExecuteAction() {  }

void FxWidget::ExecuteAltAction() {  }
