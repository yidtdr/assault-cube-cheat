#include "globals.hpp"
#include "ac.h"
#include "Vectors/Vectors.h"
#include "imgui/imgui.h"

#define SCREENWIDTH 1024
#define SCREENHEIGHT 768

#define wts(a) WorldToScreen(a, mem.getMatrix()->matrix, SCREENWIDTH, SCREENHEIGHT)

struct Box {
	ImVec2 topLeft, topRight, bottomLeft, bottomRight;
};

namespace ESP
{
	Vector3 predictHeadPos(ent* target) {
		float rotation = target->viewAngle.x;
		float dx = -sin(rotation * 0.0174533);
		float dy = cos(rotation * 0.0174533);

		return (target->head + Vector3(-dx * 0.3 + target->vel.x * settings::aimbot::targetPredict / 1000.0f, -dy * 0.3 + target->vel.y * settings::aimbot::targetPredict / 1000.0f, 0.2f));
	}

	Box calcBox(Vector3 headPos, Vector3 feetPos);
	bool isTargetAlive(ent* target);
	bool isTargetOnScreen(Vector3 headpos, Vector3 feetpos);
	bool isTargetTeammate(ent* target);
	Box calcSquareBox(Vector3 pos1, Vector3 pos2);

	bool isTargetVisible(ent* target)
	{
		ent fakeLP = ent();
		fakeLP = *mem.getLocalPlayer();
		Vector3 tmpTo = target->head;
		Vector3 from = Vector3(fakeLP.head.x, fakeLP.head.y, fakeLP.head.z);
		Vector3 to = Vector3(tmpTo.x, tmpTo.y, tmpTo.z);

		mem.AcIntersectGeometry(from, to);

		return (to.x == tmpTo.x && to.y == tmpTo.y && to.z == tmpTo.z);
	}

	void cycle() {
		if (settings::esp::bEnabled) {
			
			int playerCount = *mem.getPlayerCount();
			for (int i = 0; i < playerCount; i++) {
				ent* target = mem.getEntListPtr()->ents[i];
				if (target != NULL) {
					if (isTargetAlive(target))
					{
						Vector3 feetPos = wts(target->pos); Vector3 temp = (target->head + Vector3(0, 0, 0.65f));
						Vector3 headPos = wts(temp);
						Vector3 headRealPos = predictHeadPos(target);
						Vector3 headScreenPos = wts(headRealPos);

						if (isTargetOnScreen(headPos, feetPos)) {
							if (!(isTargetTeammate(target) && (!settings::esp::bDrawTeam))) {
								Box targetBox = calcBox(headPos, feetPos);
								ImColor col = isTargetVisible(target) ? ImColor(ImVec4(0.0f, 1.0f, 0.0f, 0.8f)) : ImColor(ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
								ImGui::GetBackgroundDrawList()->AddQuad(targetBox.topLeft, targetBox.bottomLeft, targetBox.bottomRight, targetBox.topRight, col);
								if (settings::esp::bDrawHead) {
									float radius = 160.0f / ((target->head - mem.getLocalPlayer()->head).Length());
									ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(headScreenPos.x, headScreenPos.y), radius,
										ImColor(ImVec4(1.0f, 0.0f, 0.0f, 0.8f)), 30, 2.0f);
								}
								if (settings::esp::bDrawName)
								{
									float green = 100.0f / target->hp;
									float red = 1.0f - green;
									ImGui::GetBackgroundDrawList()->AddText(
										ImVec2(feetPos.x, feetPos.y),
										ImColor(ImVec4(red, green, 0.0f, 0.7f)),
										target->name
									);
								}
							}
						}
					}
				}
			}
		}
	}

	Box calcSquareBox(Vector3 pos1, Vector3 pos2) {

		float minX = min(pos1.x, pos2.x);
		float minY = min(pos1.y, pos2.y);
		float maxX = max(pos1.x, pos2.x);
		float maxY = max(pos1.y, pos2.y);

		std::cout << "minx: " << minX << std::endl;
		std::cout << "miny: " << minY << std::endl;
		std::cout << "maxx: " << maxX << std::endl;
		std::cout << "maxy: " << maxY << std::endl;

		ImVec2 topLeft = ImVec2(minX, minY);
		ImVec2 topRight = ImVec2(maxX, minY);
		ImVec2 bottomLeft = ImVec2(minX, maxY);
		ImVec2 bottomRight = ImVec2(maxX, maxY);

		return Box{ topLeft, topRight, bottomLeft, bottomRight };
	}

	Box calcBox(Vector3 headPos, Vector3 feetPos) {
		int height = abs(headPos.y - feetPos.y);
		int width = height / 2;

		ImVec2 topLeft = ImVec2(headPos.x - width / 2, headPos.y);
		ImVec2 topRight = ImVec2(headPos.x + width / 2, headPos.y);
		ImVec2 bottomLeft = ImVec2(feetPos.x - width / 2, feetPos.y);
		ImVec2 bottomRight = ImVec2(feetPos.x + width / 2, feetPos.y);

		return Box{ topLeft, topRight, bottomLeft, bottomRight };
	}

	bool isTargetAlive(ent* target)
	{
		return ((target->hp > 0) && (target->hp <= 100));
	}

	bool isTargetOnScreen(Vector3 headpos, Vector3 feetpos) {
		return !((headpos.x == -1) && (feetpos.x == -1));
	}

	bool isTargetTeammate(ent* target) {
		return (target->team == mem.getLocalPlayer()->team);
	}
}